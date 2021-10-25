#include <string.h>

char *strcpy(char *s, const char *ct) {
    char *r = s;
    while ((*s++ = *ct++))
        ;
    return r;
}

char *strncpy(char *s, const char *ct, size_t n) {
    char *r = s;
    while (n--) {
        *s++ = *ct++;
    }
    return r;
}

char *strcat(char *s, const char *ct) {
    char *r = s;
    while (*s)
        s++;
    while ((*s++ = *ct++))
        ;
    return r;
}

char *strncat(char *s, const char *ct, size_t n) {
    char *r = s;
    while (*s)
        s++;
    while (n--) {
        *s++ = *ct++;
    };
    *s = '\0';
    return r;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    while (n-- && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

// TODO implement this when I have environment variable
int strcoll(const char *str1, const char *str2) { return 0; }

size_t strlen(const char *cs) {
    size_t i = 0;
    while (cs[i++] != '\0')
        ;
    return i;
}

char *strchr(const char *cs, int c) {
    while (*cs && *cs != c) {
        cs++;
    }
    return cs;
}

char *strrchr(const char *cs, int c) {
    const char *temp;
    while (*cs) {
        if (*cs == c) {
            temp = cs;
        }
        cs++;
    }
    return temp;
}

char *strstr(const char *cs, const char *ct);

size_t strspn(const char *cs, const char *ct);
size_t strcspn(const char *cs, const char *ct);

// TODO implement this when I understand what it's doing
size_t strxfrm(char *dest, const char *src, size_t n);
char *strpbrk(const char *cs, const char *ct);
char *strerror(int n);
char *strtok(char *s, const char *t);

void *memcpy(void *s, const void *ct, size_t n) {
    while (n--) {
        *(char *)s = *(char *)ct;
        s++;
        ct++;
    }
    return s;
}

void *memmove(void *s, const void *ct, size_t n) {
    char temp[n];
    for (size_t i = 0; i < n; i++) {
        temp[i] = ((char *)s)[i];
    }
    for (size_t i = 0; i < n; i++) {
        ((char *)ct)[i] = temp[i];
    }
    return s;
}

int memcmp(const void *cs, const void *ct, size_t n) {
    while (n-- && *(char *)cs == *(char *)ct) {
        cs++;
        ct++;
    }
    return *(char *)cs - *(char *)ct;
}

void *memchr(const void *str, int c, size_t n) {}

void *memset(void *str, int c, size_t n) {
    while (n--) {
        *(unsigned char *)str = (unsigned char) c;
    }
    return str;
}

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
