#include <arch.h>

extern void init_fpu(void);

void init_arch(void)
{
    init_fpu();
}
