#include <memory.h>

void memset(uint8_t *addr, uint8_t value, uint32_t len) {
    uint8_t *temp = addr;
    for (; len != 0; len--) {
        *temp++ = value;
    }
}

void memcpy(uint8_t *dest, uint8_t *src, uint32_t len) {
    for (uint32_t i = 0; i < len; i++) {
        *(dest + i) = *(src + i);
    }
}
