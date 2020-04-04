#ifndef X86_64_ARCH_H
#define X86_64_ARCH_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void flush_page(void *addr);

void flush_tlb(void);

void load_pml4(uint64_t addr);

#ifdef __cplusplus
}
#endif

#endif // X86_64_ARCH_H
