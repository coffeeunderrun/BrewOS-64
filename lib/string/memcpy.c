#include <string.h>

void*
memcpy(void* dst, const void* src, size_t cnt)
{
    char* pd = (char*)dst;
    char* ps = (char*)src;

    for(; cnt; cnt--, *pd++ = *ps++)
        ;

    return dst;
}
