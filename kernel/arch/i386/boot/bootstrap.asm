; Constants for multiboot header 
MBALIGN equ 1 << 0          ; align loaded module on page boundaries
MEMINFO equ 1 << 1          ; provide memory map
FLAGS equ MBALIGN | MEMINFO
MAGIC equ 0x1BADB002        ; Magic number for bootloader to find header
CHECKSUM equ -(MAGIC + FLAGS)

; Kernel Constants for higher half kernel
KERNEL_OFFSET equ 0xC0000000 ; 3 Gb
KERNEL_PAGE_NUMBER equ (KERNEL_OFFSET >> 22) ; The page directory index will be the first 10 bits

; Bootloader will search the first 8Kb of the kernel file
; Aligned at 32-bit boundaries
section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; The bootloader wil not setup stack, it is the kernel job to do so
; Stack in x86 has to be 16-byte aligned and the compiler will assume that
; the stack is properly aligned
section .bss
; Allocate stack
align 16
stack_bottom:
  ; resb is similar to db but for uninitialized data (use for .bss section)
  resb 16386 ; 16Kb
stack_top:

; Prelocate page table, allowing us to enter higher half kernel before setting up
; the real one. This one assumes that kernel is not larger than 4MB
section .data
align 4
boot_page_directory:
  times 1024 dd 0
boot_page_table1: ; Kernel code
  times 1024 dd 0
boot_page_table2: ; Kernel data space
  times 1024 dd 0

section .text
global _start:function (_kernel_start.end - _start)
_start:
  ; Setup variables
  mov edi, (boot_page_table1 - KERNEL_OFFSET) ; Address of boot_page_table1 in physical address
  mov esi, 0                                  ; Physical address of boot_page_table1 being mapped to
  mov edx, 0                                  ; boot_page_table1 index

map_page_table:
  mov ecx, esi
  ; Map as present and writable
  or ecx, 0x003
  ; Move using array indexing, each entry is an address (4 bytes)
  mov [edi+edx*4], ecx

  add esi, 4096 ; Next address 4kb
  add edx, 1    ; Next index
  cmp edx, 1024 ; Only map 1024 entries
  jl map_page_table

  mov edi, (boot_page_table2 - KERNEL_OFFSET) ; Address of boot_page_table2 in physical address
  mov esi, 0x400000                           ; Physical address of boot_page_table2 being mapped to
  mov edx, 0                                  ; boot_page_table2 index

map_page_table2:
  mov ecx, esi
  ; Map as present and writable
  or ecx, 0x003
  ; Move using array indexing, each entry is an address (4 bytes)
  mov [edi+edx*4], ecx

  add esi, 4096 ; Next address 4kb
  add edx, 1    ; Next index
  cmp edx, 1024 ; Only map 1024 entries
  jl map_page_table2

map_page_table_done:
  ; Map address 0
  mov esi, (boot_page_directory - KERNEL_OFFSET + 0)
  mov dword [esi], (boot_page_table1 - KERNEL_OFFSET + 0x003)
  ; Map address 0xC0000000 (kernel start)
  mov esi, (boot_page_directory - KERNEL_OFFSET + 768*4) ; 768 is the 10 msb of kernel address
  mov dword [esi], (boot_page_table1 - KERNEL_OFFSET + 0x003)
  ; Map next entry in kernel
  mov esi, (boot_page_directory - KERNEL_OFFSET + 769*4) 
  mov dword [esi], (boot_page_table2 - KERNEL_OFFSET + 0x003)

  ; Load page directory
  mov ecx, (boot_page_directory - KERNEL_OFFSET)
  mov cr3, ecx

  ; Enable paging
  mov ecx, cr0
  or ecx, 0x80000000
  mov cr0, ecx

  ; Do a long jump
  lea ecx, [_kernel_start]
  ; NOTE: must be an absolute jump
  jmp ecx

; The bootloader set us in protected mode, disable interrupt, disable paging
; We will have GDT and INT setup in kernel main
_kernel_start:
  ; Setup stack
	mov esp, stack_top + 16386

  push eax ; Magic number boot loader
  push ebx ; Boot loader hardward info structure
  extern main
	call main

.hang:
	hlt
	jmp .hang
.end:
