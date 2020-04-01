#include <arch.h>

extern "C" void init_fpu(void);

void init_arch(void)
{
    init_fpu();
}
