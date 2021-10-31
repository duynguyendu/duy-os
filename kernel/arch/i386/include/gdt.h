#ifndef __GDT_H__
#define __GDT_H__

#include <types.h>

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;

    uint8_t accessed : 1;
    uint8_t readable : 1;
    uint8_t conforming : 1;
    uint8_t code : 1;

    uint8_t descriptor_table : 1;
    uint8_t privilege : 2;
    uint8_t present : 1;

    uint8_t limit_middle : 4;

    uint8_t avl : 1;
    uint8_t long_mode : 1;
    uint8_t size : 1;
    uint8_t granularity : 1;

    uint8_t base_high;
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

struct gdt_table {
    uint16_t size;
    uint32_t addr;
} __attribute__((packed));
typedef struct gdt_table gdt_table_t;

void gdt_init();
#endif // !__GDT_H__
