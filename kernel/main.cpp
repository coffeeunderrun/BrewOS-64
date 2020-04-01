/*
 * Assumptions
 * - Loader disabled interrupts
 * - Loader initialized GDT
 */

#include <arch.h>
#include <interrupts.h>
#include <memory.h>

extern "C" void _init();

extern "C" void __cxa_pure_virtual() { }

extern "C" void kmain(void *mmap) __attribute__((noreturn));

void kmain(void *mmap)
{
    // Initialize architecture specific features
    init_arch();

    // Initialize interrupt handler
    init_int();

    // Initialize memory manager
    init_mem(mmap);

    // Call global constructors
    _init();

    while(1);
}
