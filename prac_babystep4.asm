[ORG 0x7c00]
  xor ax, ax
  mov ds, ax

  cld

  mov ax, 0xb800
  mov es, ax         

  mov si, msg
  call sprint

hang:
  jmp hang

dochar: call cprint
sprint:
  lodsb
  cmp al, 0
  jne dochar
  add byte [ypos], 1
  mov byte [xpos], 0
  ret

cprint:
  mov ah, 0x0f
  mov cx, ax

  movzx ax, byte [ypos]
  mov dx, 160
  mul dx
  movzx bx, byte [xpos]
  shl bx, 1

  mov di, 0
  add di, ax
  add di, bx

  mov ax, cx
  stosw
  add byte [xpos], 1

  ret

xpos db 0
ypos db 0
msg db "Why mine is not running????", 0

times 510-($-$$) db 0
db 0x55
db 0xaa
