#include <kernel/panic.h>
#include <multiboot.h>
#include <stdio.h>

long multiboot_read_bios_data(multiboot_info_t *mbd, uint32_t magic) {
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("shit not magic\n");
    }

    if (!(mbd->flags >> 6 & 0x1)) {
        panic("invalid memory map given by GRUB bootloader\n");
    }

    /* Loop through the memory map and display the values */
    int i;
    long ram_size = 0;
    for (i = 0; i < mbd->mmap_length; i += sizeof(multiboot_mmap_entry_t)) {
        multiboot_mmap_entry_t *mmmt =
            (multiboot_mmap_entry_t *)(mbd->mmap_addr + i);

        ram_size += mmmt->length;

        // if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE_RAM) {
        /*
         * Do something with this memory block!
         * BE WARNED that some of memory shown as availiable is actually
         * actively being used by the kernel! You'll need to take that
         * into account before writing to memory!
         */
        // }
    }
    return ram_size;
}
