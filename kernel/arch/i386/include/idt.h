#ifndef __IDT__
#define __IDT__

#include <types.h>

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

/* Set each entry of idt */
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);
void init_idt();
#endif // !__IDT__
