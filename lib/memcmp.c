#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n)
{
    char *p1 = (char *)s1;
    char *p2 = (char *)s2;

    for( ; n && *p1++ == *p2++; n--);

    return *p1 - *p2;
}
