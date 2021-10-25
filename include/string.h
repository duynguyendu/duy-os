#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>
#include <types.h>

char *strcpy(char *s, const char *ct);
char *strncpy(char *s, const char *ct, size_t n);

char *strcat(char *s, const char *ct);
char *strncat(char *s, const char *ct, size_t n);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
int strcoll(const char *str1, const char *str2);

size_t strlen(const char *cs);

char *strchr(const char *cs, int c);
char *strrchr(const char *cs, int c);
char *strstr(const char *cs, const char *ct);

size_t strspn(const char *cs, const char *ct);
size_t strcspn(const char *cs, const char *ct);

size_t strxfrm(char *dest, const char *src, size_t n);
char *strpbrk(const char *cs, const char *ct);
char *strerror(int n);
char *strtok(char *s, const char *t);

void *memcpy(void *s, const void *ct, size_t n);
void *memmove(void *s, const void *ct, size_t n);
int memcmp(const void *cs, const void *ct, size_t n);
void *memchr(const void *str, int c, size_t n);
void *memset(void *str, int c, size_t n);

void int_to_ascii(int n, char str[]);

#endif //! __STRING_H__
