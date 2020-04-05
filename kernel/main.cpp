/*
 * Assumptions
 * - Loader disabled interrupts
 * - Loader initialized GDT
 */

#include <interrupts.h>
#include <memory.h>

extern "C" void _init();

extern "C" void __cxa_pure_virtual() { }

namespace BrewOS {

extern "C" void KernelMain(void *mmap) __attribute__((noreturn));

void KernelMain(void *mmap)
{
    // Initialize interrupt handler
    Interrupts::Initialize();

    // Initialize memory manager
    Memory::Initialize(mmap);

    // Call global constructors
    _init();

    while(1);
}

} // BrewOS
