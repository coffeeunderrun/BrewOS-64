#include <arch/x86_64/interrupts.h>

extern void init_pics(void);
extern void load_idt(void);
extern void clear_irq(uint64_t irq);

void init_interrupts()
{
    init_pics();
    load_idt();
}

void irq_handler(irq_registers_t *regs)
{
    clear_irq(regs->vector);
}
