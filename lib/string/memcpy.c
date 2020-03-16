#include <string.h>

void *memcpy(void *d, const void *s, size_t n)
{
    char *pd = (char *)d;
    char *ps = (char *)s;

    for( ; n; n--, *pd++ = *ps++);

    return d;
}
