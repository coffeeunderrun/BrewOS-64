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

int fprintf(FILE *restrict file, const char *restrict fmt, ...);

int printf(const char *restrict fmt, ...);

int snprintf(char *restrict str, size_t size, const char *restrict fmt, ...);

int sprintf(char *restrict str, const char *restrict fmt, ...);

int vfprintf(FILE *restrict file, const char *restrict fmt, va_list arg);

int vprintf(const char *restrict fmt, va_list arg);

int vsnprintf(char *restrict str, size_t size, const char *restrict fmt, va_list arg);

int vsprintf(char *restrict str, const char *restrict fmt, va_list arg);

#endif // STDIO_H
