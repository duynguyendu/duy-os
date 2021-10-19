#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

static inline void CLI() { __asm__ volatile("cli"); }
static inline void STI() { __asm__ volatile("sti"); }

#endif // !__INTERRUPT_H__
