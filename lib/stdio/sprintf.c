#include <stdio.h>

int
snprintf(char* str, size_t size, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    int result = vsnprintf(str, size, fmt, arg);

    va_end(arg);

    return result;
}

int
sprintf(char* str, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    int result = vsprintf(str, fmt, arg);

    va_end(arg);

    return result;
}

int
vsnprintf(char* str, size_t size, const char* fmt, va_list arg)
{
    return 0;
}

int
vsprintf(char* str, const char* fmt, va_list arg)
{
    return 0;
}
