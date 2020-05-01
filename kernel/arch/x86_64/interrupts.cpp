#include <arch/x86_64/interrupts.hpp>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <interrupts.hpp>
#include <vector.hpp>

extern "C" void
init_interrupts(void);

extern "C" void
clear_irq(unsigned irq);

namespace BrewOS {
namespace Interrupts {

static const unsigned MAX_VECTORS = 48;

static Vector<Callback>* callbacks[MAX_VECTORS];

void
Initialize(void)
{
    init_interrupts();
}

void
AddCallback(unsigned vector, Callback callback)
{
    if(vector >= MAX_VECTORS) {
        return;
    }

    if(callbacks[vector] == nullptr) {
        callbacks[vector] = new Vector<Callback>();
    }

    callbacks[vector]->Push(callback);
}

void
RemoveCallback(unsigned vector, Callback callback)
{
    if(vector >= MAX_VECTORS) {
        return;
    }

    for(unsigned i = 0; callbacks[vector] && i < callbacks[vector]->Count();) {
        if(callbacks[vector]->Get(i) != callback) {
            i++;
            continue;
        }

        callbacks[vector]->Remove(i);
    }
}

extern "C" void
InterruptHandler(Registers* regs)
{
    assert(callbacks != nullptr);
    assert(regs->vector < MAX_VECTORS);

    for(auto callback : *callbacks[regs->vector]) {
        callback(regs);
    }

    clear_irq(regs->vector);
}

} // Interrupts
} // BrewOS
