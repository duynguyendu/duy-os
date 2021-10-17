[bits 16]
switch_to_pm:
  cli                   ; Disable interrupt because interrupt in 32-bit mode is different from 16-bit mode

  lgdt [gdt_descriptor] ; Load GDT

  mov eax, cr0          ; Switch to PM by set the first bit of CR0 to 1
  or eax, 0x1
  mov cr0, eax

  jmp CODE_SEG:init_pm  ; Far jmp so that CPU flush all in pipeline, avoid executing both 16-bit and 32-bit instruction

[bits 32]
init_pm:
  mov ax, DATA_SEG      ; Update data segment pointer to new one in GDT
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebp, 0x90000      ; Update stack
  mov esp, ebp

  call BEGIN_PM
