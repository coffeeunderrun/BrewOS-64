#include <string.h>

int memcmp(const void *a, const void *b, size_t cnt)
{
    char *pa = (char *)a;
    char *pb = (char *)b;

    for( ; cnt && *pa++ == *pb++; cnt--);

    return *pa - *pb;
}
