#include <string.h>

char*
strncpy(char* dst, const char* src, size_t cnt)
{
    char* pd = dst;
    const char* ps = src;

    for(; cnt && *ps; cnt--, *pd++ = *ps++)
        ;

    *pd = '\0';

    return dst;
}
