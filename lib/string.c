#include "lib/string.h"

void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0)
        str[i++] = '-';
    str[i] = '\0';

    i--;
    int len = i;
    n = i / 2;
    while (i > n) {
        char t;
        t = str[i];
        str[i] = str[len - i];
        str[len - i] = t;
        i--;
    }
}
