#include <stdio.h>

int snprintf(char *restrict str, size_t size, const char *restrict fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    int result = vsnprintf(str, size, fmt, arg);

    va_end(arg);

    return result;
}

int sprintf(char *restrict str, const char *restrict fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    int result = vsprintf(str, fmt, arg);

    va_end(arg);

    return result;
}

int vsnprintf(char *restrict str, size_t size, const char *restrict fmt, va_list arg)
{
    return 0;
}

int vsprintf(char *restrict str, const char *restrict fmt, va_list arg)
{
    return 0;
}
