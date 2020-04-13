#ifndef TIME_H
#define TIME_H

#include <stdio.h>

typedef long time_t;
typedef long suseconds_t;

typedef void* timer_t;
typedef int clockid_t;
typedef long clock_t;

struct tm
{
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
    long __tm_gmtoff;
    const char *__tm_zone;
};

#ifdef __cplusplus
extern "C" {
#endif

clock_t clock();

time_t time(time_t *);

double difftime(time_t, time_t);

time_t mktime(struct tm *);

size_t strftime(char *, size_t, const char*, const struct tm *);

char* strptime(const char* buf, const char* format, struct tm* tm);

struct tm* gmtime(const time_t *);

struct tm* localtime(const time_t *);

char* asctime(const struct tm *);

char* ctime(const time_t *);

#ifdef __cplusplus
}
#endif

#endif // TIME_H
