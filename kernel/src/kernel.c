#include <arch/interrupts.h>
#include <stdint.h>

void _kernel_main(uint64_t mb_ptr)
{
  init_interrupts();
  while (1);
}
