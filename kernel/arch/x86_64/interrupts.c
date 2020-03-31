#include <errno.h>
#include <interrupts.h>
#include <arch/x86_64/interrupts.h>

extern void init_pics(void);
extern void load_idt(void);
extern void clear_irq(uint64_t irq);

void init_int(void)
{
    init_pics();
    load_idt();
}

void isr_handler(isr_registers_t *regs)
{
    clear_irq(regs->vector);
}

err_t int_add_handler(unsigned int idx, isr_handler_t hnd)
{
    return OK;
}

err_t int_del_handler(unsigned int idx, isr_handler_t hnd)
{
    return OK;
}