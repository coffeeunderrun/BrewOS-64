#include <string.h>

char*
strcpy(char* dst, const char* src)
{
    char* pd = dst;
    const char* ps = src;

    for(; *ps; *pd++ = *ps++)
        ;

    *pd = '\0';

    return dst;
}
