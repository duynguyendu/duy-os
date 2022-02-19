#include <kernel/heap.h>

uint32_t *start_address = 0;
int heap_size = 0;

void heap_init(uint32_t *start_address_t, int heap_size_t) {
    start_address = start_address_t;
    heap_size = heap_size_t;

    *start_address = start_address + 1; // Start address block

    // Init the first free block
    header_t *header =
        (header_t *)start_address + 2; // The address of the header
    header->valid = 0;
    header->size =
        heap_size_t - 4; // 1 byte for first block and 2 bytes for header
    header->next_header = 0;
    header->prev_header = (header_t *)start_address;
}

void *kmalloc_new(int size) {
    /* Find the empty space */
    header_t *block_header = *start_address;

    // First fit
    while (block_header != 0) {
        if (block_header->valid == 0 &&
            block_header->size > (size + sizeof(header_t))) {
            break;
        }
        block_header = block_header->next_header;
    }

    if (block_header == 0) {
        // TODO request more space
        return 0;
    }

    /* TODO: recomment: this Case divided free space is big or not */
    if (!(block_header->size < (size + 2 * sizeof(header_t)))) {
        header_t *divide_free =
            (header_t *)((char *)block_header + size +
                         sizeof(header_t)); // Divide the free space
        divide_free->valid = 0;
        divide_free->size = block_header->size - size - sizeof(header_t);

        divide_free->next_header = block_header->next_header;
        // Connect to prev free
        block_header->next_header = divide_free;
        divide_free->prev_header = block_header;
    }

    return (uint32_t *)block_header + sizeof(header_t);
}

void *kcalloc(int num, int size) {
    return kmalloc_new(num * size);
}

void kfree(void *address) {
    // TODO implemente security for free()
    header_t *header = (header_t *) ((char *)address - sizeof(header_t));
    header_t *current = header;
    header_t *prev_header = header->prev_header;
    header_t *next_header = header->next_header;
    int size = header->size;

    if (header->prev_header->valid == 0) {
        size += header->prev_header->size;
        prev_header = header->prev_header->prev_header;
        current = header->prev_header;
    }

    if (header->next_header->valid == 0) {
        size += header->next_header->size;
        next_header = header->next_header->next_header;
    }

    current->valid = 0;
    current->size = size;
    current->prev_header = prev_header;
    current->next_header = next_header;
}
