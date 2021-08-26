; babystep2.1.asm
; Improve babystep2.asm by introducing procedure

  mov ax, 0x07c0 ; copy the segment address to DS
  mov ds, ax

  mov si, msg ; copy the address of the first byte of string to SI
  cld ; set the direction of string to go onward

  call bios_print ; after running bios_print, it will enter hang
hang:
  jmp hang

bios_print:
  lodsb ; load byte at address DS:SI to reg AL
  or al, al ; check if string terminated by 0
  jz done
  mov ah, 0x0e
  mov bh, 0
  int 0x10 ; call interrupt
  jmp bios_print
done:
  ret

msg db "Babystep2.1", 13, 10, 0
  ; $ is the current instruction address
  ; $$ is the first address of current section
  times 510-($-$$) db 0 ; So 510-($-$$) will return the number of bytes left to fill with 0
  db 0x55
  db 0xaa
