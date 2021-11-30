#include <drivers/keyboard.h>
#include <drivers/ps2.h>
#include <drivers/vga.h>
#include <gdt.h>
#include <idt.h>
#include <interrupt/irq0_timer.h>
#include <multiboot.h>
#include <paging.h>
#include <stdio.h>
#include <tty.h>

void main(multiboot_info_t *mbd, uint32_t magic) {
    tty_init();
    kprintf("Hello from DuyOS\n");
    long ram_size = multiboot_read_bios_data(mbd, magic);
    gdt_init();
    idt_init();
    paging_init(ram_size);
    ps2_init();

    int *a = 0x2000000;
    *a = 10;
    kprintf("This is done by page fault: %d\n", *a);
}
