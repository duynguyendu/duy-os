; define in interrupt_service_routine.c
[extern isr_handler]
[extern irq_handler]

; Common ISR code
isr_common_stub:
  ; 1. Save CPU state
	pusha        ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
	mov ax, ds   ; Lower 16-bits of eax = ds.
	push eax     ; save the data segment descriptor
	mov ax, 0x10 ; kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
  ; 2. Call C handler
	call isr_handler
	
  ; 3. Restore state
	pop eax 
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa

	add esp, 8 ; Cleans up the pushed error code and pushed ISR number
	sti
	iret       ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

; Common IRQ code. Identical to ISR code except for the 'call' 
; and the 'pop ebx'
irq_common_stub:
    pusha 
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call irq_handler ; Different than the ISR code
    pop ebx  ; Different than the ISR code
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    sti
    iret 

; Type type of isr
%macro ISR_NOERRORCODE 1
global isr%1 
isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRORCODE 1
global isr%1 
isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro

ISR_NOERRORCODE 0
ISR_NOERRORCODE 1
ISR_NOERRORCODE 2
ISR_NOERRORCODE 3
ISR_NOERRORCODE 4
ISR_NOERRORCODE 5
ISR_NOERRORCODE 6
ISR_NOERRORCODE 7
ISR_ERRORCODE 8
ISR_NOERRORCODE 9
ISR_ERRORCODE 10
ISR_ERRORCODE 11
ISR_ERRORCODE 12
ISR_ERRORCODE 13
ISR_ERRORCODE 14
ISR_NOERRORCODE 15
ISR_NOERRORCODE 16
ISR_NOERRORCODE 17
ISR_NOERRORCODE 18
ISR_NOERRORCODE 19
ISR_NOERRORCODE 20
ISR_NOERRORCODE 21
ISR_NOERRORCODE 22
ISR_NOERRORCODE 23
ISR_NOERRORCODE 24
ISR_NOERRORCODE 25
ISR_NOERRORCODE 26
ISR_NOERRORCODE 27
ISR_NOERRORCODE 28
ISR_NOERRORCODE 29
ISR_NOERRORCODE 30
ISR_NOERRORCODE 31

; IRQ 
%macro IRQ 2
global irq%1 
irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

IRQ 0, 32
IRQ 4, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 5, 36
IRQ 6, 37
IRQ 7, 38
IRQ 8, 39
IRQ 9, 40
IRQ 10, 41
IRQ 11, 42
IRQ 12, 43
IRQ 13, 44
IRQ 14, 45
IRQ 15, 46
