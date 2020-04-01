#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <sys/types.h>

typedef struct
{
} FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

#define stdin  stdin
#define stdout stdout
#define stderr stderr

int fprintf(FILE *file, const char *fmt, ...);

int printf(const char *fmt, ...);

int snprintf(char *str, size_t size, const char *fmt, ...);

int sprintf(char *str, const char *fmt, ...);

int vfprintf(FILE *file, const char *fmt, va_list arg);

int vprintf(const char *fmt, va_list arg);

int vsnprintf(char *str, size_t size, const char *fmt, va_list arg);

int vsprintf(char *str, const char *fmt, va_list arg);

#endif // STDIO_H
