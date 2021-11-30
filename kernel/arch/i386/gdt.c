#include <gdt.h>
#include <stdio.h>

gdt_entry_t gdt_entries[3];
gdt_table_t gdt_table;

extern void load_gdt(gdt_table_t *table);

void add_gdt_entry(gdt_entry_t *entry, uint32_t base, uint32_t limit,
                   uint8_t present, uint8_t privilege, uint8_t descriptor_table,
                   uint8_t code, uint8_t conforming, uint8_t readable,
                   uint8_t accessed, uint8_t granularity, uint8_t size,
                   uint8_t long_mode, uint8_t avl) {
    entry->base_low = base & 0xFFFF;
    entry->base_middle = (base >> 16) & 0xFF;
    entry->base_middle = (base >> 24) & 0xFF;

    entry->limit_low = limit & 0xFFFF;
    entry->limit_middle = (limit >> 16) & 0xF;

    entry->present = present;
    entry->privilege = privilege;
    entry->descriptor_table = descriptor_table;

    entry->code = code;
    entry->conforming = conforming;
    entry->readable = readable;
    entry->accessed = accessed;

    entry->granularity = granularity;
    entry->size = size;
    entry->long_mode = long_mode;
    entry->avl = avl;
}

void gdt_init() {
    kprintf("[GDT] Initializing\n");
    gdt_table.size = sizeof(gdt_entries) - 1;
    gdt_table.addr = &gdt_entries;

    /* Null segment */
    add_gdt_entry(&gdt_entries[0], 0, 0x0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    /* Code segment */
    add_gdt_entry(&gdt_entries[1], 0, 0xfffff, 1, 0x00, 1, 1, 0, 1, 0, 1, 1, 0,
                  0);
    /* Data segment */
    add_gdt_entry(&gdt_entries[2], 0, 0xfffff, 1, 0x00, 1, 0, 0, 1, 0, 1, 1, 0,
                  0);
    kprintf("[GDT] Loading gdt table\n");
    load_gdt(&gdt_table);
}
