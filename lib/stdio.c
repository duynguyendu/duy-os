#include <stdio.h>
#include <tty.h>

void printf(char *str) { vga_print(str); }
