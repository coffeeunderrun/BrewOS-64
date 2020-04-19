#ifndef SYS_STAT_H
#define SYS_STAT_H

#include <fcntl.h>
#include <time.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#define S_IFIFO  010000  // FIFO special
#define S_IFCHR  020000  // Character special
#define S_IFDIR  040000  // Directory
#define S_IFBLK  060000  // Block special
#define S_IFREG  0100000 // Regular
#define S_IFLNK  0120000 // Symbolic link
#define S_IFSOCK 0140000 // Socket
#define S_IFMT   0170000 // Type of file

#define S_IXOTH 01    // Execute/search permission, others
#define S_IWOTH 02    // Write permission, others
#define S_IROTH 04    // Read permission, others
#define S_IRWXO 07    // Read, write, execute/search by others
#define S_IXGRP 010   // Execute/search permission, group
#define S_IWGRP 020   // Write permission, group
#define S_IRGRP 040   // Read permission, group
#define S_IRWXG 070   // Read, write, execute/search by group
#define S_IXUSR 0100  // Execute/search permission, owner
#define S_IWUSR 0200  // Write permission, owner
#define S_IRUSR 0400  // Read permission, owner
#define S_IRWXU 0700  // Read, write, execute/search by owner
#define S_ISVTX 01000 // On directories, restricted deletion flag
#define S_ISGID 02000 // Set-group-ID on execution
#define S_ISUID 04000 // Set-user-ID on execution

#define S_ISBLK(mode)  0 // Block special file
#define S_ISCHR(mode)  0 // Character special file
#define S_ISDIR(mode)  0 // Directory
#define S_ISFIFO(mode) 0 // Pipe or FIFO special file
#define S_ISLNK(mode)  0 // Symbolic link
#define S_ISREG(mode)  0 // Regular file
#define S_ISSOCK(mode) 0 // Socket

#define S_TYPEISMQ(buf)  0 // Test for a message queue
#define S_TYPEISSEM(buf) 0 // Test for a semaphore
#define S_TYPEISSHM(buf) 0 // Test for a shared memory object
#define S_TYPEISTMO(buf) 0 // Test macro for a typed memory object

struct stat
{
    dev_t st_dev;         // Device ID
    ino_t st_ino;         // File serial number
    mode_t st_mode;       // File mode
    nlink_t st_nlink;     // Number of hard links
    uid_t st_uid;         // User ID
    gid_t st_gid;         // Group ID
    dev_t st_rdev;        // Device ID (BLK or CHR file type)
    off_t st_size;        // File size in bytes
    time_t st_atime;      // Last access timestamp
    time_t st_mtime;      // Last modify timestamp
    time_t st_ctime;      // Last change timestamp
    blksize_t st_blksize; // Block size
    blkcnt_t st_blocks;   // Block count
};

__BEGIN_DECLS

int chmod(const char *path, mode_t mode);

int fchmod(int fd, mode_t mode);

int fchmodat(int fd, const char *path, mode_t mode, int flag);

int fstat(int fd, struct stat *buf);

int fstatat(int fd, const char *__restrict path, struct stat *__restrict buf, int flag);

int futimens(int fd, const struct timespec times[2]);

int lstat(const char *__restrict path, struct stat *__restrict buf);

int mkdir(const char *path, mode_t mode);

int mkdirat(int fd, const char *path, mode_t mode);

int mkfifo(const char *path, mode_t mode);

int mkfifoat(int fd, const char *path, mode_t mode);

int mknod(const char *path, mode_t mode, dev_t dev);

int mknodat(int, const char *path, mode_t mode, dev_t dev);

int stat(const char *__restrict path, struct stat *__restrict buf);

mode_t umask(mode_t mask);

int utimensat(int fd, const char *path, const struct timespec times[2], int flag);

__END_DECLS

#endif // SYS_STAT_H
