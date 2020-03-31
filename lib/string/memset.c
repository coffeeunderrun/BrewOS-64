#include <string.h>

void *memset(void *addr, int val, size_t cnt)
{
    char *p = (char *)addr;

    for( ; cnt; cnt--, *p++ = (char)val);

    return addr;
}
