#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <sys/types.h>

typedef struct isr_registers isr_registers_t;

typedef void (*isr_handler_t)(isr_registers_t *);

#ifdef __cplusplus
extern "C" {
#endif

void init_int(void);

err_t int_add_handler(unsigned int idx, isr_handler_t hnd);

err_t int_del_handler(unsigned int idx, isr_handler_t hnd);

#ifdef __cplusplus
}
#endif

#endif // INTERRUPTS_H
