#include <string.h>

void*
memmove(void* dst, const void* src, size_t cnt)
{
    if(dst < src) {
        return memcpy(dst, src, cnt);
    }

    char* pd = (char*)((uintptr_t)dst + cnt - 1);
    char* ps = (char*)((uintptr_t)src + cnt - 1);

    for(; cnt; cnt--, *pd-- = *ps--)
        ;

    return dst;
}
