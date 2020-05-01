#include <string.h>

int
memcmp(const void* p1, const void* p2, size_t cnt)
{
    char* pp1 = (char*)p1;
    char* pp2 = (char*)p2;

    for(; cnt && *pp1++ == *pp2++; cnt--)
        ;

    return *pp1 - *pp2;
}
