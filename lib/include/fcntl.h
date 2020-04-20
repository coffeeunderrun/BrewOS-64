#ifndef FCNTL_H
#define FCNTL_H

#include <sys/cdefs.h>
#include <sys/stat.h>
#include <sys/types.h>

#define F_DUPFD         0  // Duplicate file descriptor
#define F_DUPFD_CLOEXEC 1  // Duplicate file descriptor with the close-on- exec flag FD_CLOEXEC set
#define F_GETFD         2  // Get file descriptor flags
#define F_SETFD         3  // Set file descriptor flags
#define F_GETFL         4  // Get file status flags and file access modes
#define F_SETFL         5  // Set file status flags
#define F_GETLK         6  // Get record locking information
#define F_SETLK         7  // Set record locking information
#define F_SETLKW        8  // Set record locking information; wait if blocked
#define F_GETOWN        9  // Get process or process group ID to receive SIGURG signals
#define F_SETOWN        10 // Set process or process group ID to receive SIGURG signals

#define FD_CLOEXEC 1 // Close the file descriptor upon execution of an exec family function.

#define F_RDLCK 0 // Shared or read lock.
#define F_UNLCK 1 // Unlock.
#define F_WRLCK 2 // Exclusive or write lock.

#define O_ACCMODE  (O_RDONLY | O_WRONLY | O_RDWR)

#define O_RDONLY    0x0     // Open read-only
#define O_WRONLY    0x1     // Open write-only
#define O_RDWR      0x2     // Open for reading and writing
#define O_EXEC      0x4     // Open for execute only
#define O_SEARCH    0x8     // Open directory for search only
#define O_APPEND    0x10    // Set append mode
#define O_DSYNC     0x20    // Write according to synchronized I/O data integrity completion
#define O_NONBLOCK  0x40    // Non-blocking mode
#define O_RSYNC     0x80    // Synchronized read I/O operations
#define O_SYNC      0x100   // Write according to synchronized I/O file integrity completion
#define O_CLOEXEC   0x200   // Close file descriptor on exec
#define O_CREAT     0x400   // Create file if it doesn't exist
#define O_DIRECTORY 0x800   // Fail if file is a non-directory file
#define O_EXCL      0x1000  // Fail if file already exists
#define O_NOCTTY    0x2000  // Do not assign controlling terminal
#define O_NOFOLLOW  0x4000  // Do not follow symbolic links
#define O_TRUNC     0x8000  // Truncate file to zero length
#define O_TTY_INIT  0x10000 // Set terminal parameters to conforming behavior

#define AT_EACCESS           0x1 // Check access using effective user and group ID
#define AT_SYMLINK_NOFOLLOW  0x2 // Do not follow symbolic links
#define AT_SYMLINK_FOLLOW    0x4 // Follow symbolic link
#define AT_REMOVEDIR         0x8 // Remove directory instead of file

#define AT_FDCWD -100  // Special FD to use with *at functions for current working directory

#define POSIX_FADV_DONTNEED   0 // Expect data will not be accessed
#define POSIX_FADV_NOREUSE    1 // Expect one-time access of data
#define POSIX_FADV_NORMAL     2 // No advice to give on data access
#define POSIX_FADV_RANDOM     3 // Expect random order when accessing data
#define POSIX_FADV_SEQUENTIAL 4 // Expect sequential order when accessing data
#define POSIX_FADV_WILLNEED   5 // Expect future access of data

struct flock
{
    short l_type;   // Type of lock
    short l_whence; // Flag for starting offset
    off_t l_start;  // Relative offset in bytes
    off_t l_len;    // Size; if 0 then until EOF
    pid_t l_pid;    // Process ID
};

__BEGIN_DECLS

int creat(const char *path, mode_t mode);

int fcntl(int fd, int cmd, ...);

int open(const char *path, int flag, ...);

int openat(int fd, const char *path, int flag, ...);

int posix_fadvise(int fd, off_t offset, off_t len, int advice);

int posix_fallocate(int fd, off_t offset, off_t len);

__END_DECLS

#endif // FCNTL_H
