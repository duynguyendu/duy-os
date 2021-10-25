#include <string.h>
#include <tty.h>
#include <drivers/vga.h>

void tty_init() { vga_clear_screen(); }

void tty_write(char *message) { vga_print(message); }

void tty_putchar(char c) { vga_put(c); }

void receive_key(char key, uint8_t mask) {
    // TODO masking
    if (!(mask & 8)){
        if (key == '\b') {
            vga_backspace();
            // TODO backspace
        } else {
            if (!(mask & 8) && mask & 1) {
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
