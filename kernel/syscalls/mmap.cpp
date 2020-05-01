#include <sys/mman.h>

namespace BrewOS {
namespace Syscalls {

extern "C" void*
mmap(void* addr, size_t len, int pflag, int flag, int fd, off_t offset)
{
    return nullptr;
}

} // Syscalls
} // BrewOS
