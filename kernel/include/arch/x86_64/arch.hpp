#ifndef X86_64_ARCH_H
#define X86_64_ARCH_H

#include <cstdint>

extern "C" void
flush_page(void* addr);

extern "C" void
flush_tlb(void);

extern "C" void
load_pml4(uintptr_t addr);

namespace BrewOS {

typedef struct Registers {
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
} Registers;

} // BrewOS

#endif // X86_64_ARCH_H
