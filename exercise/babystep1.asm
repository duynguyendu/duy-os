; babystep1.asm

mov ah, 0x0e
mov al, 'B'
int 0x10 ; call bios interrupt code 0x10 with ah 0x0e will print `al` to screen

hang:
  jmp hang

  times 510-($-$$) db 0
  db 0x55
  db 0xaa
