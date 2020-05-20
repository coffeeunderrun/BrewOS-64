#include <arch/x86_64/arch.hpp>
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

const int VECTOR_DE = 0;    // Divide by zero error
const int VECTOR_DB = 1;    // Debug
const int VECTOR_NMI = 2;   // NMI
const int VECTOR_BP = 3;    // Breakpoint
const int VECTOR_OF = 4;    // Overflow
const int VECTOR_BR = 5;    // Bound range
const int VECTOR_UD = 6;    // Invalid opcode
const int VECTOR_NM = 7;    // Device not available
const int VECTOR_DF = 8;    // Double fault
const int VECTOR_TS = 10;   // Invalid TSS
const int VECTOR_NP = 11;   // Segment not present
const int VECTOR_SS = 12;   // Stack
const int VECTOR_GP = 13;   // General protection
const int VECTOR_PF = 14;   // Page fault
const int VECTOR_MF = 16;   // x87 floating point
const int VECTOR_AC = 17;   // Alignment check
const int VECTOR_MC = 18;   // Machine check
const int VECTOR_XM = 19;   // SIMD floating point
const int VECTOR_VE = 20;   // Virtualization
const int VECTOR_SX = 30;   // Security
const int VECTOR_PIT = 32;  // PIT
const int VECTOR_KB = 33;   // Keyboard
const int VECTOR_COM2 = 35; // COM2
const int VECTOR_COM1 = 36; // COM1
const int VECTOR_LPT2 = 37; // LPT2
const int VECTOR_FD = 38;   // Floppy disk
const int VECTOR_LPT1 = 39; // LPT1
const int VECTOR_RTC = 40;  // RTC
const int VECTOR_PS2 = 44;  // Mouse
const int VECTOR_HDD1 = 46; // Primary hard disk
const int VECTOR_HDD2 = 47; // Secondary hard disk

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

    if(callbacks[regs->vector] != nullptr) {
        for(auto callback : *callbacks[regs->vector]) {
            callback(regs);
        }
    }

    clear_irq(regs->vector);
}

} // Interrupts
} // BrewOS
