#ifndef STDLIB_H
#define STDLIB_H

#include <sys/cdefs.h>
#include <sys/types.h>

typedef struct {
    int quot;
    int rem;
} div_t;

typedef struct {
    long quot;
    long rem;
} ldiv_t;

typedef struct {
    long long quot;
    long long rem;
} lldiv_t;

__BEGIN_DECLS

void
abort(void);

int
abs(int x);

int
atexit(void (*func)(void));

double
atof(const char* str);

int
atoi(const char* str);

long
atol(const char* str);

long long
atoll(const char* str);

void*
bsearch(const void* val, const void* arr, size_t el_cnt, size_t el_size, int (*comparator)(const void*, const void*));

void*
calloc(size_t el_cnt, size_t el_size);

div_t
div(int numer, int denom);

void
exit(int stat);

void
free(void* addr);

char*
getenv(const char* name);

long
labs(long x);

ldiv_t
ldiv(long int numer, long int denom);

long long
llabs(long long x);

lldiv_t
lldiv(long long int numer, long long int denom);

void*
malloc(size_t size);

void
qsort(void* arr, size_t el_cnt, size_t el_size, int (*comparator)(const void*, const void*));

int
rand(void);

void*
realloc(void* addr, size_t size);

void
srand(unsigned int seed);

float
strtof(const char* str, char** end_ptr);

double
strtod(const char* str, char** end_ptr);

long double
strtold(const char* str, char** end_ptr);

long
strtol(const char* str, char** end_ptr, int base);

unsigned long
strtoul(const char* str, char** end_ptr, int base);

long long
strtoll(const char* str, char** end_ptr, int base);

unsigned long long
strtoull(const char* str, char** end_ptr, int base);

int
system(const char* cmd);

__END_DECLS

#endif // STDLIB_H
