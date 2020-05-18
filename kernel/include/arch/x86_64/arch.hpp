#ifndef X86_64_ARCH_H
#define X86_64_ARCH_H

#include <cstdint>

extern "C" void
flush_page(void* addr);

extern "C" void
flush_tlb(void);

extern "C" void
load_pml4(uintptr_t addr);

#endif // X86_64_ARCH_H
