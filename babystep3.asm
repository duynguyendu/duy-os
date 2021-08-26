; babystep3.asm
; The object of babystep3 is to learn instruction encoding
mov cx, 0xff
mov ax, cx
mov cx, [temp]

temp db 0x99

times 510-($-$$) db 0
db 0x55
db 0xaa
