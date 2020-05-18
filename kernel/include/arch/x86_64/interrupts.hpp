#ifndef X86_64_INTERRUPTS_H
#define X86_64_INTERRUPTS_H

#include <cstdint>

namespace BrewOS {
namespace Interrupts {

const int VECTOR_DE = 0;    // Divide by zero error
const int VECTOR_DB = 1;    // Debug
const int VECTOR_NMI = 2;   // NMI
const int VECTOR_BP = 3;    // Breakpoint
const int VECTOR_OF = 4;    // Overflow
const int VECTOR_BR = 5;    // Bound range
const int VECTOR_UD = 6;    // Invalid opcode
const int VECTOR_NM = 7;    // Device not available
const int VECTOR_DF = 8;    // Double fault
const int VECTOR_TS = 10;   // Invalid TSS
const int VECTOR_NP = 11;   // Segment not present
const int VECTOR_SS = 12;   // Stack
const int VECTOR_GP = 13;   // General protection
const int VECTOR_PF = 14;   // Page fault
const int VECTOR_MF = 16;   // x87 floating point
const int VECTOR_AC = 17;   // Alignment check
const int VECTOR_MC = 18;   // Machine check
const int VECTOR_XM = 19;   // SIMD floating point
const int VECTOR_VE = 20;   // Virtualization
const int VECTOR_SX = 30;   // Security
const int VECTOR_PIT = 32;  // PIT
const int VECTOR_KB = 33;   // Keyboard
const int VECTOR_COM2 = 35; // COM2
const int VECTOR_COM1 = 36; // COM1
const int VECTOR_LPT2 = 37; // LPT2
const int VECTOR_FD = 38;   // Floppy disk
const int VECTOR_LPT1 = 39; // LPT1
const int VECTOR_RTC = 40;  // RTC
const int VECTOR_PS2 = 44;  // Mouse
const int VECTOR_HDD1 = 46; // Primary hard disk
const int VECTOR_HDD2 = 47; // Secondary hard disk

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

} // Interrupts
} // BrewOS

#endif // X86_64_INTERRUPTS_H
