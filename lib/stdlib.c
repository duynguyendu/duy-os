#include <stdlib.h>

int atoi(const char *str) {
    int n = 0;
    while (*str) {
        if ('0' <= *str && *str <= '9') {
            n = n * 10 + (*str - '0');
            str++;
        } else {
            // TODO exit 
            //
        }
    }
    return n;
}

void itoa(int num, char *dest) {
    int i, sign;
    if ((sign = num) < 0)
        num = -num;
    i = 0;
    do {
        dest[i++] = num % 10 + '0';
    } while ((num /= 10) > 0);

    if (sign < 0)
        dest[i++] = '-';
    dest[i] = '\0';

    i--;
    int len = i;
    num = i / 2;
    while (i > num) {
        char t;
        t = dest[i];
        dest[i] = dest[len - i];
        dest[len - i] = t;
        i--;
    }
}

void lltoa(long long num, char *dest) {
    int i, sign;
    if ((sign = num) < 0)
        num = -num;
    i = 0;
    do {
        dest[i++] = num % 10 + '0';
    } while ((num /= 10) > 0);

    if (sign < 0)
        dest[i++] = '-';
    dest[i] = '\0';

    i--;
    int len = i;
    int middle = i / 2;
    while (i > middle) {
        char t;
        t = dest[i];
        dest[i] = dest[len - i];
        dest[len - i] = t;
        i--;
    }
}
