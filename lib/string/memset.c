#include <string.h>

void*
memset(void* addr, int chr, size_t cnt)
{
    char* p = (char*)addr;

    for(; cnt; cnt--, *p++ = (char)chr)
        ;

    return addr;
}
