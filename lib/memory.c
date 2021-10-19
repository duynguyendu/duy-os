#include <memory.h>

void memset(void *addr, uint8_t value, uint32_t len) {
    char *a = addr;
    for (uint32_t i = 0; i < len; i++) {
        a[i] = value;
    }
}

void memcpy(void *dest, void *src, uint32_t len) {
    char *d = dest;
    const char *s = src;
    for (uint32_t i = 0 ; i < len; i++) {
        d[i] = s[i];
    }
}
