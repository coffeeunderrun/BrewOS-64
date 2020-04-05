#include <arch/x86_64/interrupts.h>
#include <errno.h>
#include <interrupts.h>

extern "C"
{
     void init_pics(void);
     void load_idt(void);
     void clear_irq(uint64_t irq);
}

namespace BrewOS {
namespace Interrupts {

void Initialize(void)
{
    init_pics();
    load_idt();
}

void AddHandler(int vector, Handler handler)
{
}

void RemoveHandler(int vector, Handler handler)
{
}

extern "C" void InterruptHandler(Registers *regs)
{
    clear_irq(regs->vector);
}

} // Interrupts
} // BrewOS
