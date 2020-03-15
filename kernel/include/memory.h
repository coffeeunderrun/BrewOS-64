#ifndef MEMORY_H
#define MEMORY_H

#include <types.h>

void init_memory(void *);

status_t kmalloc(addr_t, bool, bool);

status_t kfree(addr_t);

void page_fault_handler(irq_registers_t);

#endif // MEMORY_H
