#include <string.h>

char *strcat(char *dst, const char *src)
{
    char *pd = dst + strlen(dst);
    const char *ps = src;

    for ( ; *ps; *pd++ = *ps++);

    *pd = '\0';

    return dst;
}
