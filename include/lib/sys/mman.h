#ifndef SYS_MMAN_H
#define SYS_MMAN_H

#include <sys/types.h>

#define PROT_NONE  (0)
#define PROT_READ  (1 << 0)
#define PROT_WRITE (1 << 1)
#define PROT_EXEC  (1 << 2)

#define MAP_FAILED ((void *) - 1)

void *mmap(void *, size_t, int, int, int, off_t);

#endif // SYS_MMAN_H
