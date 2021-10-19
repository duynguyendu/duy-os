#include <asm/ports.h>
#include <idt.h>
#include <isr.h>
#include <stdio.h>
#include <string.h>

isr_t interrupt_handlers[256];

void isr_handler(registers_t r) {
    if (r.int_no != 31) {
        printf("received interrupt: ");
        char s[3];
        int_to_ascii(r.int_no, s);
        printf(s);
        printf("\n");
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) {
    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */
    if (r.int_no >= 40)
        port_byte_out(PIC_SLAVE_CMD, PIC_EOI); /* slave */
    port_byte_out(PIC_MASTER_CMD, PIC_EOI);    /* master */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}
