#ifndef __TTY_H__
#define __TTY_H__

#include <types.h>

struct tty_buffer_ {
    void *ptr;
    uint32_t size;
} __attribute__((packed));

typedef struct tty_buffer_ tty_buffer_t;

void tty_init();
void tty_write(char *message);
void tty_putchar(char c);

void receive_key(char key, uint8_t mask);
void receive_special_key(char special_key, uint8_t mask);

#endif // !__TTY_H__
