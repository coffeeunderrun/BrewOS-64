#ifndef KERNEL_INTERRUPTS
#define KERNEL_INTERRUPTS

#include <types.hpp>

namespace BrewOS {
namespace Interrupts {

extern const int VECTOR_DE;   // Divide by zero error
extern const int VECTOR_DB;   // Debug
extern const int VECTOR_NMI;  // NMI
extern const int VECTOR_BP;   // Breakpoint
extern const int VECTOR_OF;   // Overflow
extern const int VECTOR_BR;   // Bound range
extern const int VECTOR_UD;   // Invalid opcode
extern const int VECTOR_NM;   // Device not available
extern const int VECTOR_DF;   // Double fault
extern const int VECTOR_TS;   // Invalid TSS
extern const int VECTOR_NP;   // Segment not present
extern const int VECTOR_SS;   // Stack
extern const int VECTOR_GP;   // General protection
extern const int VECTOR_PF;   // Page fault
extern const int VECTOR_MF;   // x87 floating point
extern const int VECTOR_AC;   // Alignment check
extern const int VECTOR_MC;   // Machine check
extern const int VECTOR_XM;   // SIMD floating point
extern const int VECTOR_VE;   // Virtualization
extern const int VECTOR_SX;   // Security
extern const int VECTOR_PIT;  // PIT
extern const int VECTOR_KB;   // Keyboard
extern const int VECTOR_COM2; // COM2
extern const int VECTOR_COM1; // COM1
extern const int VECTOR_LPT2; // LPT2
extern const int VECTOR_FD;   // Floppy disk
extern const int VECTOR_LPT1; // LPT1
extern const int VECTOR_RTC;  // RTC
extern const int VECTOR_PS2;  // Mouse
extern const int VECTOR_HDD1; // Primary hard disk
extern const int VECTOR_HDD2; // Secondary hard disk

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
