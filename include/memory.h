#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <types.h>

uint32_t malloc_a(uint32_t size);
uint32_t malloc_p(uint32_t size, uint32_t *phy);
uint32_t malloc_ap(uint32_t size, uint32_t *phy);
uint32_t malloc(uint32_t size);

#endif // !__MEMORY_H__
