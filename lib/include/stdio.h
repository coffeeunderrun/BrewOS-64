#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <sys/cdefs.h>
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

__BEGIN_DECLS

void clearerr(FILE *file);

int fclose(FILE *file);

FILE *fdopen(int fd, const char *mode);

int feof(FILE *file);

int ferror(FILE *file);

int fflush(FILE *file);

int fgetc(FILE *file);

int fgetpos(FILE *file, fpos_t *pos);

char *fgets(char *str, int cnt, FILE *file);

int fileno(FILE *file);

int fprintf(FILE *file, const char *fmt, ...);

int fputc(int chr, FILE *file);

int fputs(const char *str, FILE *file);

FILE *fopen(const char *path, const char *mode);

size_t fread(void *ptr, size_t el_size, size_t el_cnt, FILE *file);

FILE *freopen(const char *path, const char *mode, FILE *file);

int fscanf(FILE *file, const char *fmt, ...);

int fseek(FILE *file, long offet, int seek);

int fsetpos(FILE *file, const fpos_t *pos);

long ftell(FILE *file);

size_t fwrite(const void *ptr, size_t el_size, size_t el_cnt, FILE *file);

int getc(FILE *file);

int getchar(void);

void perror(const char *str);

int printf(const char *fmt, ...);

int putc(int chr, FILE *file);

int putchar(int chr);

int puts(const char *str);

int remove(const char *path);

int rename(const char *old_name, const char *new_name);

void rewind(FILE *file);

int scanf(const char *fmt, ...);

void setbuf(FILE *file, char *buf);

int setvbuf(FILE *file, char *buf, int mode, size_t size);

int snprintf(char *str, size_t size, const char *fmt, ...);

int sprintf(char *str, const char *fmt, ...);

int sscanf(const char *str, const char *fmt, ...);

FILE *tmpfile(void);

int ungetc(int chr, FILE *file);

int vfprintf(FILE *file, const char *fmt, va_list arg);

int vprintf(const char *fmt, va_list arg);

int vsnprintf(char *str, size_t size, const char *fmt, va_list arg);

int vsprintf(char *str, const char *fmt, va_list arg);

__END_DECLS

#endif // STDIO_H
