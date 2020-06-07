#ifndef KERNEL_MEMORY
#define KERNEL_MEMORY

#include <cerrno>
#include <types.hpp>

namespace BrewOS {
namespace Memory {

void
Initialize(void* mmap);

err_t
Allocate(void* addr, bool exec, bool write, bool user);

err_t
Free(void* addr);

void
PageFaultHandler(Registers* regs);

} // Memory
} // BrewOS

#endif // KERNEL_MEMORY
