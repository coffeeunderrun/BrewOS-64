#include <stddef.h>
#include <stdlib.h>

void*
operator new(size_t size)
{
    return malloc(size);
}

void*
operator new[](size_t size)
{
    return malloc(size);
}

void
operator delete(void* addr)
{
    free(addr);
}

void
operator delete(void* addr, size_t size)
{
    free(addr);
}

void
operator delete[](void* addr)
{
    free(addr);
}

void
operator delete[](void* addr, size_t size)
{
    free(addr);
}
