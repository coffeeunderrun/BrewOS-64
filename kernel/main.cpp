#include <interrupts.hpp>
#include <memory.hpp>
#include <syscalls.hpp>
#include <types.hpp>

extern "C" void
_init();

namespace BrewOS {

extern "C" void
KernelMain(void* mmap) __attribute__((noreturn));

void
KernelMain(void* mmap)
{
    // Initialize memory manager
    Memory::Initialize(mmap);

    // Initialize interrupt manager
    Interrupts::Initialize();

    // Let memory manager handle page faults
    Interrupts::AddCallback(Interrupts::Vectors::PF, Memory::PageFaultHandler);

    // Call global constructors
    _init();

    // Initialize system calls
    Syscalls::Initialize();

    while(1)
        ;
}

} // BrewOS
