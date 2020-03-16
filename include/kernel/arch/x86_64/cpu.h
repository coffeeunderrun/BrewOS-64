#ifndef X86_64_CPU_H
#define X86_64_CPU_H

#include <sys/types.h>

extern void flush_page(void *);

extern void flush_tlb(void);

extern void load_pml4(uint64_t);

#endif // X86_64_CPU_H
