#include <unistd.h>

#ifdef __LIBK

extern ssize_t write(int fildes, const void *buf, size_t size);

#else

ssize_t write(int fildes, const void *buf, size_t size)
{
    return 0;
}

#endif // __LIBK
