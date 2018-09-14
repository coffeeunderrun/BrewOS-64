#include <arch/irq.h>
#include <multiboot2.h>
#include <stdint.h>

void _kernel_main(uint64_t mb_ptr)
{
  init_irq();
  while (1);
}
