#ifndef MEMORY_H
#define MEMORY_H

#include <status.h>
#include <sys/types.h>

void init_mem(void *mmap);

status_t mem_alloc(void *addr, bool exec, bool rw, bool usr);

status_t mem_free(void *addr);

#endif // MEMORY_H
