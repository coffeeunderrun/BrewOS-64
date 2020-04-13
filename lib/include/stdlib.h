#ifndef STDLIB_H
#define STDLIB_H

#include <sys/types.h>

typedef struct
{
    int quot;
    int rem;
} div_t;

typedef struct
{
    long quot;
    long rem;
} ldiv_t;

typedef struct
{
    long long quot;
    long long rem;
} lldiv_t;

#ifdef __cplusplus
extern "C" {
#endif

void abort(void);

int abs(int val);

int atexit(void (*func)(void));

double atof(const char* str);

int atoi(const char *str);

long atol(const char* str);

long long atoll(const char* str);

void* bsearch(const void* value, const void* array, size_t num_elements, size_t size, int (*comparator)(const void*, const void*));

void *calloc(size_t el_cnt, size_t el_size);

div_t div(int numer, int denom);

void exit(int stat);

void free(void *addr);

char *getenv(const char *name);

long labs(long val);

ldiv_t ldiv(long int numer, long int denom);

long long llabs(long long val);

lldiv_t lldiv(long long int numer, long long int denom);

void *malloc(size_t size);

void qsort(void* array, size_t num_elements, size_t size, int (*compar)(const void *, const void *));

int rand();

void *realloc(void *addr, size_t size);

void srand(unsigned int seed);

float strtof(const char* str, char** endptr);

double strtod(const char* str, char** endptr);

long double strtold(const char* str, char** endptr);

long strtol(const char* str, char** endptr, int base);

unsigned long strtoul(const char* str, char** endptr, int base);

long long strtoll(const char* str, char** endptr, int base);

unsigned long long strtoull(const char* str, char** endptr, int base);

int system(const char* command);

#ifdef __cplusplus
}
#endif

#endif // STDLIB_H
