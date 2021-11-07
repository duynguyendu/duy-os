#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <tty.h>
#include <stdlib.h>

#define UPPER_CASE 1
#define LOWER_CASE 0

void print_hex(int num, int _case) {
    char hex_num[] = "0123456789abcdef";
    char hex_num_upper[] = "0123456789ABCDEF";
    char str[8];
    int i;
    if (_case == UPPER_CASE) {
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

void print_hex_long(long long num, int _case) {
    char hex_num[] = "0123456789abcdef";
    char hex_num_upper[] = "0123456789ABCDEF";
    char str[16];
    int i;
    if (_case == UPPER_CASE) {
        for (i = 0; i < 16; i++, num >>= 4) {
            str[i] = hex_num_upper[num & 0xF];
        }
    } else {
        for (i = 0; i < 16; i++, num >>= 4) {
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
    long long llval;
    double dval;

    va_start(ap, fmt);
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
        } else {
            char t;
            switch (*++p) {
            case 'd':
                ival = va_arg(ap, int);
                char num[10];
                itoa(ival, num);
                tty_write(num);
                break;
            case 'x':
                ival = va_arg(ap, int);
                print_hex(ival, LOWER_CASE);
                break;
            case 'X':
                ival = va_arg(ap, int);
                print_hex(ival, UPPER_CASE);
                break;
            case 'l':
                t = *++p;
                if (t == 'x') {
                    llval = va_arg(ap, long long);
                    print_hex_long(llval, LOWER_CASE);
                } else if (t == 'd') {
                    llval = va_arg(ap, long long);
                    char num[20];
                    lltoa(llval, num);
                    tty_write(num);
                } else {
                    // TODO fix this stuff
                    tty_write("fuck you, no %l something\n");
                    while (1)
                        ;
                }
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
