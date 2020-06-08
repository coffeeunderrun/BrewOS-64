#ifndef KERNEL_INTERRUPTS
#define KERNEL_INTERRUPTS

#include <types.hpp>

namespace BrewOS {
namespace Interrupts {
namespace Vectors {

extern const int DE;   // Divide by zero error
extern const int DB;   // Debug
extern const int NMI;  // NMI
extern const int BP;   // Breakpoint
extern const int OF;   // Overflow
extern const int BR;   // Bound range
extern const int UD;   // Invalid opcode
extern const int NM;   // Device not available
extern const int DF;   // Double fault
extern const int TS;   // Invalid TSS
extern const int NP;   // Segment not present
extern const int SS;   // Stack segent fault
extern const int GP;   // General protection fault
extern const int PF;   // Page fault
extern const int MF;   // x87 floating point
extern const int AC;   // Alignment check
extern const int MC;   // Machine check
extern const int XM;   // SIMD floating point
extern const int VE;   // Virtualization
extern const int SX;   // Security
extern const int PIT;  // PIT
extern const int KBD;  // Keyboard
extern const int COM2; // COM2
extern const int COM1; // COM1
extern const int LPT2; // LPT2
extern const int FDD;  // Floppy disk
extern const int LPT1; // LPT1
extern const int RTC;  // RTC
extern const int PS2;  // Mouse
extern const int HDD1; // Primary hard disk
extern const int HDD2; // Secondary hard disk

} // Vectors

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
