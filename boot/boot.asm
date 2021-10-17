[org 0x7c00]
KERNEL_OFFSET equ 0x1000

mov [BOOT_DRIVE], dl

mov bp, 0x8000
mov ss, bp

; Print loading string
mov si, BOOT_STR
call print_string

; Load kernel
call load_kernel

; Switch to protected mode
call switch_to_pm

jmp $

%include "boot/print/print_string.asm"
%include "boot/disk/disk_load.asm"
%include "boot/pm/switch_to_pm.asm"
%include "boot/pm/gdt.asm"

[bits 16]
load_kernel:
  ; Loading 1 sector to ES:BX (0x0:0x1000)
  mov bx, KERNEL_OFFSET
  mov dh, 1
  mov dl, [BOOT_DRIVE]
  call disk_load
  ret

[bits 32]
BEGIN_PM:
  call KERNEL_OFFSET
  jmp $

; Global data
BOOT_DRIVE: db 0
BOOT_STR: db "Loading boot sector", 13, 10, 0
SWITCHING_TO_PM_STR: db "Switching to protected mode", 13, 10, 0

; Filling the rest of sector
times 510-($-$$) db 0
dw 0xaa55
