#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <types.h>
void memset(void *addr, uint8_t value, uint32_t len);
void memcpy(void *dest, void *src, uint32_t len);

#endif // !__MEMORY_H__
