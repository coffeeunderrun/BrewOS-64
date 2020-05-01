#ifndef SYS_TIME_H
#define SYS_TIME_H

#include <sys/cdefs.h>
#include <sys/types.h>

struct timeval {
    time_t tv_sec;       // Seconds
    suseconds_t tv_usec; // Microseconds
};

struct itimerval {
    struct timeval it_interval; // Timer interval
    struct timeval it_value;    // Current value
};

int
utimes(const char* path, const struct timeval times[2]);

#endif // SYS_TIME_H
