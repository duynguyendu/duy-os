#include <memory.h>

// Kernel space allocation
uint32_t placement_address = 0xC0400000;

/* This kmalloc is not using heap */
// TODO rewrite code when heap is enable
uint32_t kmalloc_a(uint32_t size) {
    if (placement_address & ~0xFFFFF000) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    return kmalloc(size);
}

uint32_t kmalloc_p(uint32_t size, uint32_t *phy) {
    if (phy) {
        *phy = placement_address;
    }
    return kmalloc(size);
}

uint32_t kmalloc_ap(uint32_t size, uint32_t *phy) {
    if (placement_address & ~0xFFFFF000) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phy) {
        *phy = placement_address;
    }
    return kmalloc(size);
}

uint32_t kmalloc(uint32_t size) {
    if (placement_address & ~0xFFFFF000) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    uint32_t tmp = placement_address;
    placement_address += size;
    return tmp;
}
