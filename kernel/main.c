/*
 * Assumptions
 * - Loader disabled interrupts
 * - Loader initialized GDT
 * - Loader initialized FPU
 */

#include <interrupts.h>
#include <memory.h>

void kmain(void *mmap)
{
    // Initialize interrupt handler
    init_int();

    // Initialize memory manager
    init_mem(mmap);

    while(1);
}
