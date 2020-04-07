bits 64

extern gdt.kern_code, gdt.user_code

CPU_EXFEAT equ 0x80000001
MSR_EFER   equ 0xC0000080
MSR_STAR   equ 0xC0000081
MSR_LSTAR  equ 0xC0000082
MSR_SFMASK equ 0xC0000084

section .text

global init_syscalls
init_syscalls:
    mov eax, CPU_EXFEAT
    cpuid
    bt edx, 11 ; Check system call extentions are supported
    ; TODO: Include fallback option if they're not supported? Are they always supported on x86_64?
    jnc .done

    mov ecx, MSR_EFER
    rdmsr
    bts eax, 0 ; Enable system call extentions
    wrmsr

    mov ecx, MSR_STAR
    mov dx, gdt.user_code ; Bits 48 - 63 = user CS selector
    shl edx, 16
    mov dx, gdt.kern_code ; Bits 32 - 47 = kernel CS selector
    wrmsr

    mov ecx, MSR_LSTAR
    mov rax, qword syscall_stub ; EAX needs to contain the lower 32 bits
    mov rdx, qword syscall_stub ; EDX needs to contain the upper 32 bits
    shr rdx, 32
    wrmsr

    mov ecx, MSR_SFMASK
    rdmsr
    mov eax, 0x600 ; Mask direction and interrupt flags
    wrmsr
.done:
    ret

syscall_stub:
    o64 sysret ; Make sure 64 bit operand is used to remain in long mode
