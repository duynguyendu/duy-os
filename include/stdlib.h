#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <stddef.h>

typedef struct {
    int quote;
    int rem;
} div_t;

typedef struct {
    long int quote;
    long int rem;
} ldiv_t;

// TODO fully implemnt this

int atoi(const char *str);
void itoa(int num, char *dest);
void lltoa(long long num, char *dest);

#endif // !__STDLIB_H__
