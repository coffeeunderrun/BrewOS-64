#ifndef SYS_STAT_H
#define SYS_STAT_H

#include <time.h>
#include <sys/types.h>

typedef uint32_t mode_t;

struct stat
{
    dev_t st_dev;
    ino_t st_ino;
    mode_t st_mode;
    nlink_t st_nlink;
    uid_t st_uid;
    gid_t st_gid;
    off_t st_size;
    time_t st_atime;
    time_t st_mtime;
    time_t st_ctime;
    blksize_t st_blksize;
    blkcnt_t st_blocks;
};

#define S_IXOTH 01
#define S_IWOTH 02
#define S_IROTH 03
#define S_IRWXO 07
#define S_IXGRP 010
#define S_IWGRP 020
#define S_IRGRP 040
#define S_IRWXG 070
#define S_IXUSR 0100
#define S_IWUSR 0200
#define S_IRUSR 0400
#define S_IRWXU 0700

#define S_ISREG(mode) 1
#define S_ISDIR(mode) 0
#define S_ISLNK(mode) 0

#ifdef __cplusplus
extern "C" {
#endif

int stat(const char *pathname, struct stat *buf);

int fstat(int fd, struct stat* buf);

int lstat(const char *pathname, struct stat *buf);

int chmod(const char *pathname, mode_t mode);

int fchmod(int fd, mode_t mode);

mode_t umask(mode_t mask);

int mkdir(const char *path, mode_t mode);

#ifdef __cplusplus
}
#endif


#endif // SYS_STAT_H
