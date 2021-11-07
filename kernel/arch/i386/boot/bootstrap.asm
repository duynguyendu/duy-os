; Constants for multiboot header 
MBALIGN equ 1 << 0          ; align loaded module on page boundaries
MEMINFO equ 1 << 1          ; provide memory map
FLAGS equ MBALIGN | MEMINFO
MAGIC equ 0x1BADB002        ; Magic number for bootloader to find header
CHECKSUM equ -(MAGIC + FLAGS)

; bootloader will search the first 8Kb of the kernel file
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
align 16
stack_bottom:
  ; resb is similar to db but for uninitialized data (use for .bss section)
  resb 16386 ; 16Kb
stack_top:

section .text
global _start:function (_start.end - _start)

; The bootloader set us in protected mode, disable interrupt, disable paging
; We will have GDT and INT setup in kernel main
_start:
	mov esp, stack_top

  push eax
  push ebx
  extern main
	call main
.hang:
	hlt
	jmp .hang
.end:
