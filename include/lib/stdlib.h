#ifndef STDLIB_H
#define STDLIB_H

#include <sys/types.h>

void *calloc(size_t el_cnt, size_t el_size);

void free(void *addr);

void *malloc(size_t size);

void *realloc(void *addr, size_t size);

#endif // STDLIB_H
