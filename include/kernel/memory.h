#ifndef MEMORY_H
#define MEMORY_H

#include <interrupts.h>
#include <status.h>
#include <sys/types.h>

void init_memory(void *);

status_t kmalloc(void *, bool, bool, bool);

status_t kfree(void *);

status_t page_fault_handler(irq_registers_t *);

#endif // MEMORY_H
