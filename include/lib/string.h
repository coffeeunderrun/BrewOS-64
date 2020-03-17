#ifndef STRING_H
#define STRING_H

#include <sys/types.h>

int memcmp(const void *a, const void *b, size_t cnt);

void *memcpy(void *dst, const void *src, size_t cnt);

void *memmove(void *dst, const void *src, size_t cnt);

void *memset(void *addr, int val, size_t cnt);

#endif // STRING_H
