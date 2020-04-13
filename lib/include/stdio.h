#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <sys/types.h>

typedef off_t fpos_t;

typedef struct
{
} FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

#define stdin  stdin
#define stdout stdout
#define stderr stderr

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 3

#define EOF -1

#define BUFSIZ 0x2000

#define _IOFBF 1 // full buffering
#define _IOLBF 2 // line buffering
#define _IONBF 3 // no buffering

#ifdef __cplusplus
extern "C" {
#endif

void clearerr(FILE* stream);

int fclose(FILE *file);

FILE* fdopen(int fd, const char *mode);

int feof(FILE* stream);

int ferror(FILE *stream);

int fflush(FILE *file);

int fgetc(FILE* stream);

int fgetpos(FILE* stream, fpos_t* pos);

char* fgets(char* s, int n, FILE* stream);

int fileno(FILE* stream);

int fprintf(FILE *file, const char *fmt, ...);

int fputc(int c, FILE* stream);

int fputs(const char* s, FILE* stream);

FILE *fopen(const char *path, const char *mode);

size_t fread(void *ptr, size_t size, size_t cnt, FILE *file);

FILE* freopen(const char* filename, const char* mode, FILE* stream);

int fscanf(FILE* stream, const char* format, ...);

int fseek(FILE *file, long off, int seek);

int fsetpos(FILE* stream, const fpos_t* pos);

long ftell(FILE *file);

size_t fwrite(const void *ptr, size_t size, size_t cnt, FILE *file);

int getc(FILE* stream);

int getchar();

void perror(const char* s);

int printf(const char *fmt, ...);

int putc(int c, FILE* stream);

int putchar(int c);

int puts(const char *s);

int remove(const char *filename);

int rename(const char *old_name, const char *new_name);

void rewind(FILE* stream);

int scanf(const char* format, ...);

void setbuf(FILE *file, char *buf);

int setvbuf(FILE* stream, char* buf, int mode, size_t size);

int snprintf(char *str, size_t size, const char *fmt, ...);

int sprintf(char *str, const char *fmt, ...);

int sscanf(const char* s, const char* format, ...);

FILE* tmpfile();

int ungetc(int c, FILE* stream);

int vfprintf(FILE *file, const char *fmt, va_list arg);

int vprintf(const char *fmt, va_list arg);

int vsnprintf(char *str, size_t size, const char *fmt, va_list arg);

int vsprintf(char *str, const char *fmt, va_list arg);

#ifdef __cplusplus
}
#endif

#endif // STDIO_H
