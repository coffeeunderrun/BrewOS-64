#ifndef CPU_H
#define CPU_H

#include <stdint.h>

static inline void load_pml4(uint64_t addr)
{
    asm volatile ("mov cr3, %0" :: "r"(addr) : "memory");
}

static inline void flush_tlb()
{
    asm volatile ("mov rax, cr3" ::: "rax", "memory");
    asm volatile ("mov cr3, rax" ::: "rax", "memory");
}

static inline void flush_page(void *p)
{
    asm volatile ("invlpg [%0]" :: "r"(p) : "memory");
}

#endif // CPU_H
