#include <asm/ports.h>
#include <drivers/keyboard.h>
#include <idt.h>
#include <isr.h>
#include <stdio.h>
#include <string.h>

isr_t interrupt_handlers[256];

void isr0_handler(registers_t r) { printf("No 0 ples\n"); }
void irq1_handler(registers_t r) { printf("testing keyboard interrupt\n"); }

void install_isr() {
    register_interrupt_handler(0, isr0_handler);
    keyboard_init();

    /* Hardware interrupt */
    /* 0: Divide by 0 */
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    /* 1: Debug exception */
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    /* 2: Non maskable interrupt exception */
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    /* 3: Int 3 exception */
    idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
    /* 4: Into Exception */
    idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
    /*  5: Out of Bounds Exception */
    idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
    /*  6: Invalid Opcode Exception */
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    /*  7: Coprocessor Not Available Exception */
    idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
    /*  8: Double Fault Exception (With Error Code!) */
    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
    /*  9: Coprocessor Segment Overrun Exception */
    idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
    /*  10: Bad TSS Exception (With Error Code!) */
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    /*  11: Segment Not Present Exception (With Error Code!) */
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    /*  12: Stack Fault Exception (With Error Code!) */
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    /*  13: General Protection Fault Exception (With Error Code!) */
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    /*  14: Page Fault Exception (With Error Code!) */
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    /*  15: Reserved Exception */
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    /*  16: Floating Point Exception */
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    /*  17: Alignment Check Exception */
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    /*  18: Machine Check Exception */
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    /*  19: Reserved */
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    /*  20: Reserved */
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    /*  21: Reserved */
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    /*  22: Reserved */
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    /*  23: Reserved */
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    /*  24: Reserved */
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    /*  25: Reserved */
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    /*  26: Reserved */
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    /*  27: Reserved */
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    /*  28: Reserved */
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    /*  29: Reserved */
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    /*  30: Reserved */
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    /*  31: Reserved */
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    /* Remap the PIC */
    port_byte_out(PIC_MASTER_CMD, 0x11);
    port_byte_out(PIC_SLAVE_CMD, 0x11);
    /* Master offset to INT 0x20 */
    port_byte_out(PIC_MASTER_DATA, 0x20);
    /* Slave offset to INT 0x28 */
    port_byte_out(PIC_SLAVE_DATA, 0x28);
    /* All of the below I don't know */
    port_byte_out(PIC_MASTER_DATA, 0x04);
    port_byte_out(PIC_SLAVE_DATA, 0x02);
    port_byte_out(PIC_MASTER_DATA, 0x01);
    port_byte_out(PIC_SLAVE_DATA, 0x01);
    port_byte_out(PIC_MASTER_DATA, 0x0);
    port_byte_out(PIC_SLAVE_DATA, 0x0);

    /* Install the IRQs */
    /* 0: System timer -> 32 */
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    /* 1: Keyboard on PS/2 port -> 33 */
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    /* 2: Cascaded signals from IRQs 8–15 (actually using IRQ 9) -> 34 */
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    /* 3: Serial port controller for serial port 2 -> 35 */
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    /* 4: Serial port controller for serial port 1 -> 36 */
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void isr_handler(registers_t r) {
    printf("YES\n");
    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}

void irq_handler(registers_t r) {
    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    } else {
        send_eoi(r);
    }
}

void send_eoi(registers_t r) {
    if (r.int_no >= 40)
        port_byte_out(PIC_SLAVE_CMD, PIC_EOI); /* slave */
    port_byte_out(PIC_MASTER_CMD, PIC_EOI);    /* master */
}
