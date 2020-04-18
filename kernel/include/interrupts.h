#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <sys/types.h>

namespace BrewOS {
namespace Interrupts {

typedef struct Registers Registers;

typedef void (*Callback)(Registers *regs);

void Initialize(void);

void AddCallback(unsigned vector, Callback callback);

void RemoveCallback(unsigned vector, Callback callback);

} // Interrupts
} // BrewOS

#endif // INTERRUPTS_H
