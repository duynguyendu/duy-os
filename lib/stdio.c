#include <stdio.h>
#include <tty.h>

void printf(char *str) { tty_write(str); }
