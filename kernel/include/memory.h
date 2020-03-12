#ifndef MEMORY_H
#define MEMORY_H

#include <status.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint64_t addr_t;

void init_memory(void *mmap);

status_t kmalloc(addr_t vaddr, bool write, bool user);

status_t kfree(addr_t vaddr);

#endif // MEMORY_H
