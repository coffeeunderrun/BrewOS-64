#include <string.h>

void *memset(void *s, int c, size_t n)
{
    for(char *p = (char *)s; n; *p++ = (char)c, n--);
    return s;
}
