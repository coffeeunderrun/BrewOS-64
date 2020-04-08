#include <unistd.h>

namespace BrewOS {
namespace Syscalls {

extern "C"
ssize_t write(int fildes, const void *buf, size_t size)
{
    return 0;
}

} // Syscalls
} // BrewOS
