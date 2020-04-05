#ifndef MEMORY_H
#define MEMORY_H

#include <sys/types.h>

namespace BrewOS {
namespace Memory {

void Initialize(void *mem_map);

err_t Allocate(void *addr, bool execute, bool write, bool user);

err_t Free(void *addr);

} // Memory
} // BrewOS

#endif // MEMORY_H
