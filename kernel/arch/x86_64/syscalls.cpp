#include "syscalls.h"

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