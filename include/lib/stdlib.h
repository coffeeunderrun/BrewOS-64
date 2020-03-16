#ifndef STDLIB_H
#define STDLIB_H

#include <sys/types.h>

void *calloc(size_t, size_t);

void free(void *);

void *malloc(size_t);

void *realloc(void *, size_t);

#endif // STDLIB_H
