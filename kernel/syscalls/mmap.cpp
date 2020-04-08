#include <sys/mman.h>

namespace BrewOS {
namespace Syscalls {

extern "C"
void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
{
    return nullptr;
}

} // Syscalls
} // BrewOS
