#ifndef FCNTL_H
#define FCNTL_H

#include <sys/stat.h>
#include <sys/types.h>

#define	O_RDONLY  0x1                   // Open read-only
#define	O_WRONLY  0x2                   // Open write-only
#define	O_CREAT   0x10                  // Create file if it doesn't exist
#define	O_EXCL    0x20                  // Fail if file already exists
#define	O_TRUNC   0x10000               // Truncate file to zero length
#define	O_RDWR    (O_RDONLY | O_WRONLY) // Open for reading and writing
#define	O_ACCMODE O_RDWR                // Mask for file access modes

#ifdef __cplusplus
extern "C" {
#endif

int open(const char* path, int flags, ...);

#ifdef __cplusplus
}
#endif

#endif // FCNTL_H
