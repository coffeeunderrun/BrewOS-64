bits 64

extern kmain

section .text

global entry
entry:
    mov rsp, qword kernel_stack_top
    jmp kmain

section .bss

align 0x1000
resb 0x2000
kernel_stack_top:
