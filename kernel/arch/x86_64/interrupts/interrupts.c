#include <arch/interrupts.h>
#include <stdint.h>

typedef struct irq_regs
{
  uint64_t ds;
  uint64_t rdi, rsi, rbp, rsp, rdx, rcx, rbx, rax;
  uint64_t irq, code;
  uint64_t rip, cs, eflags, user_esp, ss;
} irq_regs_t;

extern void _init_pics();
extern void _load_idt();
extern void _clear_hardware_irq(uint64_t irq);

void init_interrupts()
{
  _init_pics();
  _load_idt();
}

void _irq_handler(irq_regs_t regs)
{
  _clear_hardware_irq(regs.irq);
}
