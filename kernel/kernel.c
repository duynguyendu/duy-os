#include "drivers/screen.h"
#include "interrupt/idt.h"
#include "interrupt/isr.h"

void main() {
    init_idt();
    clear_screen();

    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");
}
