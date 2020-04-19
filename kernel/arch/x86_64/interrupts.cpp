#include <cerrno>
#include <cstddef>
#include <cstring>
#include <interrupts.h>
#include <vector.h>
#include <arch/x86_64/interrupts.h>

extern "C"
{
    void init_interrupts(void);
    void clear_irq(unsigned irq);
}

namespace BrewOS {
namespace Interrupts {

static const unsigned MAX_VECTORS = 48;

static Vector<Callback> *callbacks[MAX_VECTORS];

void Initialize(void)
{
    init_interrupts();
}

void AddCallback(unsigned vector, Callback callback)
{
    if(vector >= MAX_VECTORS)
    {
        return;
    }

    if(callbacks[vector] == nullptr)
    {
        callbacks[vector] = new Vector<Callback>(1);
    }

    callbacks[vector]->Push(callback);
}

void RemoveCallback(unsigned vector, Callback callback)
{
    if(vector >= MAX_VECTORS)
    {
        return;
    }

    for(unsigned i = 0; callbacks[vector] && i < callbacks[vector]->GetCount(); )
    {
        if(callbacks[vector]->Get(i) != callback)
        {
            i++;
            continue;
        }

        callbacks[vector]->Remove(i);
    }
}

extern "C"
void InterruptHandler(Registers *regs)
{
    if(regs->vector >= MAX_VECTORS)
    {
        // PANIC
        return;
    }

    for(unsigned i = 0; callbacks[regs->vector] && i < callbacks[regs->vector]->GetCount(); )
    {
        callbacks[regs->vector]->Get(i)(regs);
    }

    clear_irq(regs->vector);
}

} // Interrupts
} // BrewOS
