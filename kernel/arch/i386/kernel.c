#include <drivers/keyboard.h>
#include <drivers/ps2.h>
#include <drivers/vga.h>
#include <interrupt/irq0_timer.h>
#include <paging.h>
#include <stdio.h>
#include <tty.h>

void main() {
    /* init_timer(100); */
    paging_init();
    tty_init();
    ps2_init();
    disable_cursor();
    kprintf("%dHello\n", 12);
    enable_cursor();

    int *ptr = 0xA0000000;
    int f = *ptr;

    /* __asm__ __volatile__("int $0"); */
}
