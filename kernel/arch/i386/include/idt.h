#ifndef __IDT__
#define __IDT__

#include <types.h>

/* PIC ports */
#define PIC_MASTER_CMD 0x0020
#define PIC_MASTER_DATA 0x0021
#define PIC_SLAVE_CMD 0x00A0
#define PIC_SLAVE_DATA 0x00A1

#define PIC_EOI 0x20

/* PIC remap to */
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

/* Interrupt table entry structure */
struct idt_entry_struct {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero; // It has to be zero
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

/* Localtion of idt */
struct idt_ptr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

void init_idt();

/* Declartion for isr (define in interrutp.asm) */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

/* IRQ definitions */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif // !__IDT__
