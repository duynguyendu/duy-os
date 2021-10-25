#include <memory.h>

uint32_t placement_address = 0x2000;

/* This malloc is not using heap */
// TODO rewrite code when heap is enable
uint32_t malloc_a(uint32_t size) {
    if (placement_address & ~0xFFFFF000) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    return malloc(size);
}

uint32_t malloc_p(uint32_t size, uint32_t *phy) {
    if (phy) {
        *phy = placement_address;
    }
    return malloc(size);
}

uint32_t malloc_ap(uint32_t size, uint32_t *phy) {
    if (placement_address & ~0xFFFFF000) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phy) {
        *phy = placement_address;
    }
    return malloc(size);
}

uint32_t malloc(uint32_t size) {
    if (placement_address & ~0xFFFFF000) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    uint32_t tmp = placement_address;
    placement_address += size;
    return tmp;
}
