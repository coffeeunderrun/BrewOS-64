#ifndef SYS_MMAN_H
#define SYS_MMAN_H

#include <sys/cdefs.h>
#include <sys/types.h>

#define PROT_NONE 0x0  // Page cannot be accessed
#define PROT_READ 0x1  // Page can be read
#define PROT_WRITE 0x2 // Page can be written
#define PROT_EXEC 0x4  // Page can be executed

#define MAP_FAILED ((void*)-1)

__BEGIN_DECLS

void*
mmap(void* addr, size_t len, int pflag, int flag, int fd, off_t offset);

__END_DECLS

#endif // SYS_MMAN_H
