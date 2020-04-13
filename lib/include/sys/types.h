#ifndef SYS_TYPES_H
#define SYS_TYPES_H

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef int err_t;

typedef int64_t off_t;

typedef int64_t pid_t;

typedef int64_t ssize_t;

typedef uint32_t mode_t;

typedef uint32_t ino_t;
typedef uint32_t dev_t;

typedef int nlink_t;
typedef int uid_t;
typedef int gid_t;
typedef int blksize_t;
typedef int blkcnt_t;

#endif // SYS_TYPES_H
