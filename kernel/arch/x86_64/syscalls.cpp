#include "syscalls.hpp"

extern "C"
void init_syscalls(void);

namespace BrewOS {
namespace Syscalls {

void Initialize(void)
{
    init_syscalls();
}

}
}
