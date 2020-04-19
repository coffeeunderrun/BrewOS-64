#include <sys/mman.h>

#ifdef __LIBK

extern void *mmap(void *addr, size_t len, int pflag, int flag, int fd, off_t offset);

#else

void *mmap(void *addr, size_t len, int pflag, int flag, int fd, off_t offset)
{
    return NULL;
}

#endif // __LIBK
