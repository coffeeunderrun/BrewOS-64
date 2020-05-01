#include <stdio.h>

int
printf(const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    int result = vprintf(fmt, arg);

    va_end(arg);

    return result;
}

int
vprintf(const char* fmt, va_list arg)
{
    return vfprintf(stdout, fmt, arg);
}
