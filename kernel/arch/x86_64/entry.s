bits 64

extern init_fpu, KernelMain

section .text

global entry
entry:
    mov rsp, qword kernel_stack_top

    call init_fpu

    jmp KernelMain

section .bss

align 0x1000
resb 0x2000
kernel_stack_top:
