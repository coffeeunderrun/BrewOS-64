#include <interrupts.h>
#include <memory.h>
#include <syscalls.h>

extern "C"
void _init();

namespace BrewOS {

extern "C"
void KernelMain(void *mmap) __attribute__((noreturn));

void KernelMain(void *mmap)
{
    // Initialize interrupt handler
    Interrupts::Initialize();

    // Initialize memory manager
    Memory::Initialize(mmap);

    // Call global constructors
    _init();

    // Initialize system calls
    Syscalls::Initialize();

    while(1);
}

} // BrewOS
