#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <types.h>
void memset(uint8_t *addr, uint8_t value, uint32_t len);
void memcpy(uint8_t *dest, uint8_t *src, uint32_t len);

#endif // !__MEMORY_H__
