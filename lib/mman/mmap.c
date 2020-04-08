#include <sys/mman.h>

#ifdef __LIBK

extern void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);

#else

void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
{
    return NULL;
}

#endif // __LIBK
