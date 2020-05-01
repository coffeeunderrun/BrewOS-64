#include <string.h>

void*
memchr(const void* ptr, int chr, size_t cnt)
{
    char* p = (char*)ptr;

    for(; cnt; p[cnt--] = (char)chr)
        ;

    return p;
}
