#include <stdio.h>

int
fprintf(FILE* file, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    int result = vfprintf(file, fmt, arg);

    va_end(arg);

    return result;
}

int
vfprintf(FILE* file, const char* fmt, va_list arg)
{
    return 0;
}
