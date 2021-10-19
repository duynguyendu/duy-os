#ifndef __IRQ0_IMTER_H__
#define __IRQ0_IMTER_H__

#include <types.h>

#define PIT_CMD 0x43
#define PIT_DATA0 0x40

#define REPEAT_MODE 0x36

void init_timer(uint32_t frequency);

#endif // !__IRQ0_IMTER_H__
