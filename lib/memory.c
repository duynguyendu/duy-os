#include "lib/memory.h"

void memset(uint32_t *addr, uint8_t value, uint16_t len) {
    uint32_t *temp = addr;
    for (; len != 0; len--) {
        *temp++ = value;
    }
}
