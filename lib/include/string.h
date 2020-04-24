#ifndef STRING_H
#define STRING_H

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

void *memchr(const void *ptr, int chr, size_t cnt);

int memcmp(const void *p1, const void *p2, size_t cnt);

void *memcpy(void *dst, const void *src, size_t cnt);

void *memmove(void *dst, const void *src, size_t cnt);

void *memset(void *addr, int chr, size_t cnt);

char *strcat(char *dst, const char *src);

char *strchr(const char *str, int chr);

int strcmp(const char *s1, const char *s2);

int strcoll(const char *s1, const char *s2);

char *strcpy(char *dst, const char *src);

size_t strcspn(const char *s1, const char *s2);

char *strerror(int err);

size_t strlen(const char *str);

char *strncat(char *dst, const char *src, size_t cnt);

int strncmp(const char *s1, const char *s2, size_t cnt);

char *strncpy(char *dst, const char *src, size_t cnt);

char *strpbrk(const char *s1, const char *s2);

char *strrchr(const char *str, int chr);

size_t strspn(const char *s1, const char *s2);

char *strstr(const char *s1, const char *s2);

char *strtok(char *dst, const char *src);

size_t strxfrm(char *dst, const char *src, size_t cnt);

__END_DECLS

#endif // STRING_H
