#ifndef ERRNO_H
#define ERRNO_H

#include <sys/types.h>

#define OK              0  // No error
#define E2BIG           1  // Argument list too long
#define EACCES          2  // Permission denied
#define EADDRINUSE      3  // Address in use
#define EADDRNOTAVAIL   4  // Address not available
#define EAFNOSUPPORT    5  // Address family not supported
#define EAGAIN          6  // Resource unavailable, try again (may be the same value as [EWOULDBLOCK])
#define EALREADY        7  // Connection already in progress
#define EBADF           8  // Bad file descriptor
#define EBADMSG         9  // Bad message
#define EBUSY           10 // Device or resource busy
#define ECANCELED       11 // Operation canceled
#define ECHILD          12 // No child processes
#define ECONNABORTED    13 // Connection aborted
#define ECONNREFUSED    14 // Connection refused
#define ECONNRESET      15 // Connection reset
#define EDEADLK         16 // Resource deadlock would occur
#define EDESTADDRREQ    17 // Destination address required
#define EDOM            18 // Mathematics argument out of domain of function
#define EDQUOT          19 // Reserved
#define EEXIST          20 // File exists
#define EFAULT          21 // Bad address
#define EFBIG           22 // File too large
#define EHOSTUNREACH    23 // Host is unreachable
#define EIDRM           24 // Identifier removed
#define EILSEQ          25 // Illegal byte sequence
#define EINPROGRESS     26 // Operation in progress
#define EINTR           27 // Interrupted function
#define EINVAL          28 // Invalid argument
#define EIO             29 // I/O error
#define EISCONN         30 // Socket is connected
#define EISDIR          31 // Is a directory
#define ELOOP           32 // Too many levels of symbolic links
#define EMFILE          33 // File descriptor value too large
#define EMLINK          34 // Too many links
#define EMSGSIZE        35 // Message too large
#define EMULTIHOP       36 // Reserved
#define ENAMETOOLONG    37 // Filename too long
#define ENETDOWN        38 // Network is down
#define ENETRESET       39 // Connection aborted by network
#define ENETUNREACH     40 // Network unreachable
#define ENFILE          41 // Too many files open in system
#define ENOBUFS         42 // No buffer space available
#define ENODATA         43 // [OB XSR] [Option Start] No message is available on the STREAM head read queue. [Option End // [ENODEV // No such device
#define ENOENT          44 // No such file or directory
#define ENOEXEC         45 // Executable file format error
#define ENOLCK          46 // No locks available
#define ENOLINK         47 // Reserved
#define ENOMEM          48 // Not enough space
#define ENOMSG          49 // No message of the desired type
#define ENOPROTOOPT     50 // Protocol not available
#define ENOSPC          51 // No space left on device
#define ENOSR           52 // [OB XSR] [Option Start] No STREAM resources. [Option End // [ENOSTR // [OB XSR] [Option Start] Not a STREAM. [Option End // [ENOSYS // Functionality not supported
#define ENOTCONN        53 // The socket is not connected
#define ENOTDIR         54 // Not a directory or a symbolic link to a directory
#define ENOTEMPTY       55 // Directory not empty
#define ENOTRECOVERABLE 56 // State not recoverable
#define ENOTSOCK        57 // Not a socket
#define ENOTSUP         58 // Not supported (may be the same value as [EOPNOTSUPP])
#define ENOTTY          59 // Inappropriate I/O control operation
#define ENXIO           60 // No such device or address
#define EOPNOTSUPP      61 // Operation not supported on socket (may be the same value as [ENOTSUP])
#define EOVERFLOW       62 // Value too large to be stored in data type
#define EOWNERDEAD      63 // Previous owner died
#define EPERM           64 // Operation not permitted
#define EPIPE           65 // Broken pipe
#define EPROTO          66 // Protocol error
#define EPROTONOSUPPORT 67 // Protocol not supported
#define EPROTOTYPE      68 // Protocol wrong type for socket
#define ERANGE          69 // Result too large
#define EROFS           70 // Read-only file system
#define ESPIPE          71 // Invalid seek
#define ESRCH           72 // No such process
#define ESTALE          73 // Reserved
#define ETIME           74 // [OB XSR] [Option Start] Stream ioctl() timeout. [Option End // [ETIMEDOUT // Connection timed out
#define ETXTBSY         75 // Text file busy
#define EWOULDBLOCK     76 // Operation would block (may be the same value as [EAGAIN])
#define EXDEV           77 // Cross-device link

extern int errno;
#define errno errno

#endif // ERRNO_H
