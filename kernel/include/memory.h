#ifndef MEMORY_H
#define MEMORY_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void init_mem(void *mmap);

err_t mem_alloc(void *addr, bool exec, bool rw, bool usr);

err_t mem_free(void *addr);

#ifdef __cplusplus
}
#endif

#endif // MEMORY_H
