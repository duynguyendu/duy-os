#include <drivers/keyboard.h>
#include <drivers/ps2.h>
#include <drivers/vga.h>
#include <gdt.h>
#include <idt.h>
#include <interrupt/irq0_timer.h>
#include <paging.h>
#include <stdio.h>
#include <tty.h>

void main() {
    tty_init();
    gdt_init();
    idt_init();
    paging_init();
    ps2_init();
}
