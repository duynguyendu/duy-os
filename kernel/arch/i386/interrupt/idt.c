#include <asm/ports.h>
#include <idt.h>
#include <isr.h>
#include <stdio.h>

/* Allocate idt on static memory */
idt_entry_t idt_entry[256];
idt_ptr_t idt_prt;

/* Initialize idt */
void idt_init() {
    /* Set pointer to table */
    kprintf("[IDT] Initializing\n");
    idt_prt.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_prt.base = (uint32_t)&idt_entry;

    kprintf("[IDT] Installing ISR\n");
    install_isr();

    kprintf("[IDT] Loading IDT table\n");
    /* Tell system location of IDT */
    __asm__ __volatile__("lidt (%0)" : : "r"(&idt_prt));
    /* Enable interrupt */
    __asm__ __volatile__("sti");
}

/* Set each entry in idt */
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector,
                  uint8_t flags) {
    idt_entry[num].base_low = base & 0xFFFF;
    idt_entry[num].base_high = (base >> 16) & 0xFFFF;
    idt_entry[num].selector = selector;
    idt_entry[num].zero = 0;
    idt_entry[num].flags = flags;
}
