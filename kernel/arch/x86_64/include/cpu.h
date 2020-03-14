#ifndef CPU_H
#define CPU_H

#include <stdint.h>

struct irq_registers
{
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t vector;
    uint64_t errno;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

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
