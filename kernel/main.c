/*
 * Assumptions
 * - Loader disabled interrupts
 * - Loader initialized GDT
 * - Loader initialized FPU
 */

#include <interrupts.h>
#include <memory.h>

void kmain(void *mmp)
{
    init_interrupts();

    init_memory(mmp);

    while(1);
}
