#ifndef MEMORY_H
#define MEMORY_H

#include <status.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint64_t addr_t;

void init_memory(void *);

status_t kmalloc(addr_t, bool, bool);

status_t kfree(addr_t);

#endif // MEMORY_H
