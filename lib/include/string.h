#ifndef STRING_H
#define STRING_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memchr(const void* mem, int value, size_t num);

int memcmp(const void *a, const void *b, size_t cnt);

void *memcpy(void *dst, const void *src, size_t cnt);

void *memmove(void *dst, const void *src, size_t cnt);

void *memset(void *addr, int val, size_t cnt);

char *strcat(char *dst, const char *src);

char *strchr(const char *str, int chr);

int strcmp(const char* str_a, const char* str_b);

int strcoll(const char* str_a, const char* str_b);

char *strcpy(char *dst, const char *src);

size_t strcspn(const char* str_a, const char* str_b);

char* strerror(int errno);

size_t strlen(const char *str);

char* strncat(char* dest, const char* src, size_t max);

int strncmp(const char* str_a, const char* str_b, size_t max);

char* strncpy(char* dest, const char* src, size_t max);

char* strpbrk(const char* str_a, const char* str_b);

char* strrchr(const char* str, int value);

size_t strspn(const char* str_a, const char* str_b);

char* strstr(const char* str_a, const char* str_b);

char* strtok(char* dest, const char* src);

size_t strxfrm(char* dest, const char* src, size_t num);

#ifdef __cplusplus
}
#endif

#endif // STRING_H
