#include <string.h>

void *memset(void *s, int c, size_t n)
{
    char *p = (char *)s;

    for( ; n; n--, *p++ = (char)c);

    return s;
}
