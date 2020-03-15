#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <cpu.h>
#include <status.h>

typedef struct irq_registers irq_registers_t;

void init_interrupts(void);

#endif // INTERRUPTS_H
