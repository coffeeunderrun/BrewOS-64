#ifndef X86_64_ARCH_H
#define X86_64_ARCH_H

#include <sys/types.h>

extern void flush_page(void *addr);

extern void flush_tlb(void);

extern void load_pml4(uint64_t addr);

#endif // X86_64_ARCH_H
