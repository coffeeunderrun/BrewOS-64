#include <string.h>

void *memmove(void *d, const void *s, size_t n)
{
    if(d < s)
    {
        return memcpy(d, s, n);
    }

    char *pd = (char *)(d + n - 1);
    char *ps = (char *)(s + n - 1);

    for( ; n; n--, *pd-- = *ps--);

    return d;
}
