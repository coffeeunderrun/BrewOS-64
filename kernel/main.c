/*
 * Assumptions
 * - Loader disabled interrupts
 * - Loader initialized GDT
 */

#include <arch.h>
#include <interrupts.h>
#include <memory.h>

void kmain(void *mmap)
{
    // Initialize architecture specific features
    init_arch();

    // Initialize interrupt handler
    init_int();

    // Initialize memory manager
    init_mem(mmap);

    while(1);
}
