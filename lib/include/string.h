#ifndef STRING_H
#define STRING_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void *a, const void *b, size_t cnt);

void *memcpy(void *dst, const void *src, size_t cnt);

void *memmove(void *dst, const void *src, size_t cnt);

void *memset(void *addr, int val, size_t cnt);

char *strcat(char *dst, const char *src);

char *strchr(const char *str, int chr);

char *strcpy(char *dst, const char *src);

size_t strlen(const char *str);

#ifdef __cplusplus
}
#endif

#endif // STRING_H
