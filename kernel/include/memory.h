#ifndef MEMORY_H
#define MEMORY_H

#include <interrupts.h>
#include <sys/types.h>

void init_memory(addr_t);

status_t kmalloc(addr_t, bool, bool);

status_t kfree(addr_t);

status_t page_fault_handler(irq_registers_t *);

#endif // MEMORY_H
