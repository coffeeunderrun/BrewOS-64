/*
 * Assumptions
 * - Loader disabled interrupts
 * - Loader initialized GDT
 * - Loader initialized FPU
 */

#include <interrupts.h>
#include <memory.h>

void kmain(addr_t mmap)
{
    init_interrupts();

    init_memory(mmap);

    while(1);
}
