; Loading DH sectors to ES:BX from DL drive
; Arguments:
;   dh: number of sectors to load
;   dl: disk drive

disk_load:
  push dx        ; Store dx, later used for testing if
                 ; the number of requested sectors equal the number of read sectores

  mov ah, 0x02   ; int 0x13/ah=0x02 => BIOS read sector
  mov al, dh     ; Number of sector to read
  mov ch, 0x00   ; Cylinder 0
  mov dh, 0x00   ; Head 0
  mov cl, 0x02   ; From sector 2

  int 0x13       ; Interrupt to read from disk

  jc disk_error  ; Reading error

  pop dx         ; Pop back value of dx and test
  cmp al, dh     ; AL the number of read sectors, DH the number of requested sectors
  jne disk_error

  ret

disk_error:
  mov si, DISK_ERR_MSG
  call print_string
  ret

DISK_ERR_MSG: db "Disk Error!", 13, 10, 0

