#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <sys/types.h>

namespace BrewOS {
namespace Interrupts {

typedef struct Registers Registers;

typedef void (*Handler)(Registers *regs);

void Initialize(void);

void AddHandler(int vector, Handler handler);

void RemoveHandler(int vector, Handler handler);

} // Interrupts
} // BrewOS

#endif // INTERRUPTS_H
