#include <drivers/vga.h>
#include <stdio.h>
#include <string.h>
#include <tty.h>
#include <drivers/keyboard.h>

void tty_init() {
    vga_clear_screen();
    kprintf("[TTY] Initializing\n");
}

void tty_write(char *message) { vga_print(message); }

void tty_putchar(char c) { vga_put(c); }

void receive_key(char key, uint8_t mask) {
    // TODO masking
    if (!(mask & RELEASE_MASK)) {
        if (key == '\b') {
            vga_backspace();
            // TODO backspace
        } else if (key == '\n') {
            // TODO execute something
            tty_putchar('$');
            tty_putchar(key);
        } else {
            if (mask & CONTROL_MASK) {
                tty_putchar('^');
            }
            /* TODO shortcut, print, ... */
            tty_putchar(key);
        }
    }
}

void receive_special_key(char special_key, uint8_t mask) {
    if (special_key == 0) {
        tty_write("<F1>");
    }
}
