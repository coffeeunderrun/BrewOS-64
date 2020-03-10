#include <string.h>

void *memcpy(void *d, const void *s, size_t n)
{
    for(char *pd = (char *)d, *ps = (char *)s; n; *pd++ = *ps++, n--);
    return d;
}
