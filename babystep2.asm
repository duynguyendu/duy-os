; babystep2.asm
; We will use the BIOS to print msg to screen
; Review:
;   * Boot sector loaded by the BIOS is 512 bytes
;   * The code in the boot sector is loaded at 0000:7c00 or 07c0:0000
;   * Machine starts in real mode (16 bits)
  mov ax, 0x07c0 ; copy the segment address to DS
  mov ds, ax

  mov si, msg ; copy the address of the first byte of string to SI
  cld ; set the direction of string to go onward
ch_loop:
  lodsb ; load byte at address DS:SI to reg AL
  or al, al ; check if string terminated by 0
  jz hang
  mov ah, 0x0e
  mov bh, 0
  int 0x10 ; call interrupt
  jmp ch_loop

hang:
  jmp hang

msg db "Hello world", 13, 10, 0
  ; $ is the current instruction address
  ; $$ is the first address of current section
  times 510-($-$$) db 0 ; So 510-($-$$) will return the number of bytes left to fill with 0
  db 0x55
  db 0xaa
