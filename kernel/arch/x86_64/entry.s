bits 64

global entry

extern main

section .text

entry:
    mov rsp, qword kernel_stack_top
    jmp main

section .bss

align 0x1000
resb 0x2000
kernel_stack_top:
