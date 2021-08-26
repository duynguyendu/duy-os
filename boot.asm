  mov ax, 0x07c0
  mov ds, ax

  mov si, msg
  cld
ch_loop:
  lodsb
  or al, al
  jz hang
  mov ah, 0x0e
  mov bh, 0
  int 0x10
  jmp ch_loop

hang:
  jmp hang

msg db 'Hello, World', 13, 10, 0
  times 510-($-$$) db 0
  db 0x55
  db 0xaa

