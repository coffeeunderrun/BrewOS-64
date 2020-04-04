#ifndef X86_64_INTERRUPTS_H
#define X86_64_INTERRUPTS_H

#include <sys/types.h>

#define ISR_VECTOR_DE   0  // Divide by zero error
#define ISR_VECTOR_DB   1  // Debug
#define ISR_VECTOR_NMI  2  // NMI
#define ISR_VECTOR_BP   3  // Breakpoint
#define ISR_VECTOR_OF   4  // Overflow
#define ISR_VECTOR_BR   5  // Bound range
#define ISR_VECTOR_UD   6  // Invalid opcode
#define ISR_VECTOR_NM   7  // Device not available
#define ISR_VECTOR_DF   8  // Double fault
#define ISR_VECTOR_TS   10 // Invalid TSS
#define ISR_VECTOR_NP   11 // Segment not present
#define ISR_VECTOR_SS   12 // Stack
#define ISR_VECTOR_GP   13 // General protection
#define ISR_VECTOR_PF   14 // Page fault
#define ISR_VECTOR_MF   16 // x87 floating point
#define ISR_VECTOR_AC   17 // Alignment check
#define ISR_VECTOR_MC   18 // Machine check
#define ISR_VECTOR_XM   19 // SIMD floating point
#define ISR_VECTOR_VE   20 // Virtualization
#define ISR_VECTOR_SX   30 // Security
#define ISR_VECTOR_PIT  32 // PIT
#define ISR_VECTOR_KB   33 // Keyboard
#define ISR_VECTOR_COM2 35 // COM2
#define ISR_VECTOR_COM1 36 // COM1
#define ISR_VECTOR_LPT2 37 // LPT2
#define ISR_VECTOR_FD   38 // Floppy disk
#define ISR_VECTOR_LPT1 39 // LPT1
#define ISR_VECTOR_RTC  40 // RTC
#define ISR_VECTOR_PS2  44 // Mouse
#define ISR_VECTOR_HDD1 46 // Primary hard disk
#define ISR_VECTOR_HDD2 47 // Secondary hard disk

typedef struct isr_registers
{
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
} isr_registers_t;

#endif // X86_64_INTERRUPTS_H
