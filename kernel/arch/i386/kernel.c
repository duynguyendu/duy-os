#include "boot/interrupt/idt.h"
#include "boot/interrupt/isr.h"
#include <drivers/vga.h>

void main() {
    init_idt();
    /* clear_screen(); */

    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");
}
