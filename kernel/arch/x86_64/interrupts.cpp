#include "arch.hpp"
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
namespace Vectors {

const int DE = 0;    // Divide by zero error
const int DB = 1;    // Debug
const int NMI = 2;   // NMI
const int BP = 3;    // Breakpoint
const int OF = 4;    // Overflow
const int BR = 5;    // Bound range
const int UD = 6;    // Invalid opcode
const int NM = 7;    // Device not available
const int DF = 8;    // Double fault
const int TS = 10;   // Invalid TSS
const int NP = 11;   // Segment not present
const int SS = 12;   // Stack segent fault
const int GP = 13;   // General protection fault
const int PF = 14;   // Page fault
const int MF = 16;   // x87 floating point
const int AC = 17;   // Alignment check
const int MC = 18;   // Machine check
const int XM = 19;   // SIMD floating point
const int VE = 20;   // Virtualization
const int SX = 30;   // Security
const int PIT = 32;  // PIT
const int KBD = 33;  // Keyboard
const int COM2 = 35; // COM2
const int COM1 = 36; // COM1
const int LPT2 = 37; // LPT2
const int FDD = 38;  // Floppy disk
const int LPT1 = 39; // LPT1
const int RTC = 40;  // RTC
const int PS2 = 44;  // Mouse
const int HDD1 = 46; // Primary hard disk
const int HDD2 = 47; // Secondary hard disk

} // Vectors

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
