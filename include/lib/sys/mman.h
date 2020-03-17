#ifndef SYS_MMAN_H
#define SYS_MMAN_H

#include <sys/types.h>

#define PROT_NONE  (0)
#define PROT_READ  (1 << 0)
#define PROT_WRITE (1 << 1)
#define PROT_EXEC  (1 << 2)

#define MAP_FAILED ((void *) - 1)

void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);

#endif // SYS_MMAN_H
