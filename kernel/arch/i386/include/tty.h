#ifndef __TTY_H__
#define __TTY_H__

#include <drivers/vga.h>
#include <types.h>

struct tty_buffer_ {
    void *ptr;
    uint32_t size;
} __attribute__((packed));

typedef struct tty_buffer_ tty_buffer_t;

void tty_init();
void tty_write(char *message);
void tty_putchar(char c);

void recieve_key(char key);

#endif // !__TTY_H__
