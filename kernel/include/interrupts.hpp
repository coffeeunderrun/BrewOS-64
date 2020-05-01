#ifndef KERNEL_INTERRUPTS
#define KERNEL_INTERRUPTS

namespace BrewOS {
namespace Interrupts {

typedef struct Registers Registers;

typedef void (*Callback)(Registers* regs);

void
Initialize(void);

void
AddCallback(unsigned vector, Callback callback);

void
RemoveCallback(unsigned vector, Callback callback);

} // Interrupts
} // BrewOS

#endif // KERNEL_INTERRUPTS
