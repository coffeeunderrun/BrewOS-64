/*
 * Assumptions
 * - Loader disabled interrupts
 * - Loader initialized GDT
 */

#include <interrupts.h>
#include <memory.h>

void kmain(void *mmap)
{
    init_interrupts();

    init_memory(mmap);

    while(1);
}
