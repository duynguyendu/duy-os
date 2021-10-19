#ifndef __ISR__
#define __ISR__

#include <types.h>

typedef struct {
    /* Data segment selector */
    uint32_t ds;
    /* Pushed by pusha. */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    /* Interrupt number and error code (if applicable) */
    uint32_t int_no, err_code;
    /* Pushed by the processor automatically */
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

void isr_handler(registers_t r);

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(uint8_t n, isr_t handler);

#endif // !__ISR__
