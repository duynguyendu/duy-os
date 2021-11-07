#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <tty.h>

void print_hex(int num, int upper_case) {
    char hex_num[] = "0123456789abcdef";
    char hex_num_upper[] = "0123456789ABCDEF";
    char str[10];
    int i;
    if (upper_case) {
        for (i = 0; i < 8; i++, num >>= 4) {
            str[i] = hex_num_upper[num & 0xF];
        }
    } else {
        for (i = 0; i < 8; i++, num >>= 4) {
            str[i] = hex_num[num & 0xF];
        }
    }
    i--;
    while (i >= 0) {
        putchar(str[i--]);
    }
}

void putchar(char c) { tty_putchar(c); }

void printf(char *str) { tty_write(str); }
void kprintf(char *fmt, ...) {
    va_list ap;
    char *p, *sval;
    int ival;
    double dval;

    va_start(ap, fmt);
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
        } else {
            switch (*++p) {
            case 'd':
                ival = va_arg(ap, int);
                char num[10];
                itoa(ival, num);
                tty_write(num);
                break;
            case 'x':
                ival = va_arg(ap, int);
                print_hex(ival, 0);
                break;
            case 'X':
                ival = va_arg(ap, int);
                print_hex(ival, 1);
                break;
            case 's':
                for (sval = (va_arg(ap, char *)); *sval; sval++) {
                    putchar(*sval);
                }
                break;
            }
        }
    }
    va_end(ap);
}
