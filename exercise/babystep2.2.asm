; babystep2.2.asm
; Improve babystep2.1.asm by introducing macro

%macro BiosPrint 1
    mov si, word %1
  ch_loop:
    lodsb
    or al, al
    jz done
    mov ah, 0x0e
    mov bh, 0
    int 0x10
    jmp ch_loop
  done:
%endmacro

[ORG 0x7c00] ; Using address 0000:7c00
  xor ax, ax
  mov ds, ax
  cld ; set the direction of string to go onward

  BiosPrint msg ; provide argument for macro
hang:
  jmp hang

msg db "Babystep2.2", 13, 10, 0
  ; $ is the current instruction address
  ; $$ is the first address of current section
  times 510-($-$$) db 0 ; So 510-($-$$) will return the number of bytes left to fill with 0
  db 0x55
  db 0xaa
