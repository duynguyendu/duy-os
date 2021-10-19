[bits 32]
[extern main]
[extern init_idt]

call init_idt
call main
jmp $
