#include <stdio.h>

int fprintf(FILE *restrict file, const char *restrict fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    int result = vfprintf(file, fmt, arg);

    va_end(arg);

    return result;
}

int vfprintf(FILE *restrict file, const char *restrict fmt, va_list arg)
{
    return 0;
}
