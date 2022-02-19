#ifndef __HEAP_H__
#define __HEAP_H__

#include <types.h>

enum valid { free = 0, occupied = 1 };

struct header {
    uint32_t valid : 1;
    uint32_t size : 31;
    struct header *next_header;
    struct header *prev_header;
} __attribute__((packed));
typedef struct header header_t;

void heap_init(uint32_t *start_address, int heap_size);
void *kmalloc_new(int size);
void *kcalloc(int num, int size);
void kfree(void *address);

#endif // !__HEAP_H__
