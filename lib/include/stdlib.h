#ifndef STDLIB_H
#define STDLIB_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void abort(void);

int atexit(void (*func)(void));

int atoi(const char *str);

void *calloc(size_t el_cnt, size_t el_size);

void exit(int stat);

void free(void *addr);

char *getenv(const char *name);

void *malloc(size_t size);

void *realloc(void *addr, size_t size);

#ifdef __cplusplus
}
#endif

#endif // STDLIB_H
