#ifndef STRING_H
#define STRING_H

#include <sys/types.h>

int memcmp(const void *, const void *, size_t);

void *memcpy(void *, const void *, size_t);

void *memmove(void *, const void *, size_t);

void *memset(void *, int, size_t);

#endif // STRING_H
