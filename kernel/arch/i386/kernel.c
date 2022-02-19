#include <drivers/keyboard.h>
#include <drivers/pci.h>
#include <drivers/ps2.h>
#include <drivers/vga.h>
#include <gdt.h>
#include <idt.h>
#include <interrupt/irq0_timer.h>
#include <kernel/heap.h>
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
    vga_clear_screen();
    pci_init();

    char *temp = (char *)kmalloc_new(10);
    temp[0] = 'a';
    temp[1] = 'b';
    temp[2] = '\0';
    kprintf("%s", temp);
    kfree(temp);
}
