#ifndef TIME_H
#define TIME_H

#include <stdio.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#define CLOCKS_PER_SEC ((__clock_t)1000000)

#define CLOCK_REALTIME           0 // System-wide real time clock
#define CLOCK_MONOTONIC          1 // System-wide monotonic clock
#define CLOCK_PROCESS_CPUTIME_ID 2 // Process specific CPU-time clock
#define CLOCK_THREAD_CPUTIME_ID  3 // Thread specific CPU-time clock

#define TIMER_ABSTIME 1 // Absolute time flag

struct timespec
{
    time_t tv_sec; // Seconds
    long tv_nsec;  // Nanoseconds
};

struct itimerspec
{
    struct timespec it_interval; // Timer period
    struct timespec it_value;    // Timer expiration
};

struct tm
{
    int tm_sec;   // Seconds
    int tm_min;   // Minutes
    int tm_hour;  // Hours
    int tm_mday;  // Day of the month
    int tm_mon;   // Month of the year
    int tm_year;  // Years since 1900
    int tm_wday;  // Day of the week
    int tm_yday;  // Day of the year
    int tm_isdst; // Daylight savings
};

extern int daylight;

extern long timezone;

extern char *tzname[];

__BEGIN_DECLS

char *asctime(const struct tm *time);

clock_t clock(void);

char *ctime(const time_t *time);

double difftime(time_t t1, time_t t2);

struct tm *gmtime(const time_t *time);

struct tm *localtime(const time_t * time);

time_t mktime(struct tm *time);

size_t strftime(char *__restrict ptr, size_t size, const char *__restrict fmt, const struct tm *__restrict time);

char *strptime(const char *__restrict buf, const char *__restrict fmt, struct tm *__restrict time);

time_t time(time_t *time);

__END_DECLS

#endif // TIME_H
