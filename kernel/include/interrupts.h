#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <sys/types.h>

typedef struct irq_registers irq_registers_t;

typedef status_t (*irq_handler_t)(irq_registers_t *);

void init_interrupts(void);

#endif // INTERRUPTS_H
