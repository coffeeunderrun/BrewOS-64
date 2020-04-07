#include <arch/x86_64/interrupts.h>
#include <errno.h>
#include <interrupts.h>

extern "C"
{
    void init_interrupts(void);
    void clear_irq(uint64_t irq);
}

namespace BrewOS {
namespace Interrupts {

void Initialize(void)
{
    init_interrupts();
}

void AddCallback(int vector, Callback callback)
{
}

void RemoveCallback(int vector, Callback callback)
{
}

extern "C"
void InterruptHandler(Registers *regs)
{
    clear_irq(regs->vector);
}

} // Interrupts
} // BrewOS
