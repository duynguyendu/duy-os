; Print string to screen using BIOS interrupt
; Arguments:
;   si: the pointer to string

print_string:
  mov ah, 0x0e         ; int 0x10/ ah=0x0e => BIOS print value of al to screen

print_char:
  mov al, [si]         ; Move the current char to al, prepare to print

  cmp al, 0            ; Finish if end of string
  je print_string_done

  int 0x10             ; Calling interrupt
  add si, 1            ; Move to next char
  jmp print_char

print_string_done:
  ret
