#include <unistd.h>

namespace BrewOS {
namespace Syscalls {

extern "C" ssize_t
write(int fd, const void* buf, size_t size)
{
    return 0;
}

} // Syscalls
} // BrewOS
