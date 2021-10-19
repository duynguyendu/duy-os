#include <drivers/vga.h>

void main() {
    vga_clear_screen();
    vga_print("heello");

    __asm__ __volatile__("int $2");
}
