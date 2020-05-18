#include <string.h>

int
strcmp(const char* s1, const char* s2)
{
    const char* p1 = s1;
    const char* p2 = s2;

    for(; *p1 == *p2; p1++, p2++) {
        if(!*p1) {
            return 0;
        }
    }

    return p1 - s1 + 1;
}
