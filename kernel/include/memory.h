#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

typedef uint64_t addr_t;

void init_memory(void *mmap);

#endif // MEMORY_H
