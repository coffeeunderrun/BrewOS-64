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

#define SEEK_CUR 0
#define SEEK_END 1
#define SEEK_SET 2

#ifdef __cplusplus
extern "C" {
#endif

int fclose(FILE *file);

int fflush(FILE *file);

int fprintf(FILE *file, const char *fmt, ...);

FILE *fopen(const char *path, const char *mode);

size_t fread(void *ptr, size_t size, size_t cnt, FILE *file);

int fseek(FILE *file, long off, int seek);

long ftell(FILE *file);

size_t fwrite(const void *ptr, size_t size, size_t cnt, FILE *file);

int printf(const char *fmt, ...);

void setbuf(FILE *file, char *buf);

int snprintf(char *str, size_t size, const char *fmt, ...);

int sprintf(char *str, const char *fmt, ...);

int vfprintf(FILE *file, const char *fmt, va_list arg);

int vprintf(const char *fmt, va_list arg);

int vsnprintf(char *str, size_t size, const char *fmt, va_list arg);

int vsprintf(char *str, const char *fmt, va_list arg);

#ifdef __cplusplus
}
#endif

#endif // STDIO_H
