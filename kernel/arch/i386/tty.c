#include <string.h>
#include <tty.h>

void tty_init() { vga_clear_screen(); }

void tty_write(char *message) { vga_print(message); }

void tty_putchar(char c) { vga_put(c); }

void recieve_key(char key) {
    /* TODO shortcut, print, ... */
    tty_putchar(key);
}
