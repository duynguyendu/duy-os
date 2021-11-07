#include <kernel/panic.h>

void panic(char *str) {
    kprintf(str);
    while (1)
        ;
}
