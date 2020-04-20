#ifndef MEMORY_H
#define MEMORY_H

#include <cerrno>

namespace BrewOS {
namespace Memory {

void Initialize(void *mmap);

err_t Allocate(void *addr, bool exec, bool write, bool user);

err_t Free(void *addr);

} // Memory
} // BrewOS

#endif // MEMORY_H
