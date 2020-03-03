bits 64

global entry, kernel_pml4, kernel_pdp, kernel_pd, kernel_pt

extern main

section .text

entry:
    mov rsp, qword kernel_stack_top
    jmp main

section .bss

align 0x1000
resb 0x2000
kernel_stack_top:

kernel_pml4: resb 0x1000
kernel_pdp:  resb 0x1000
kernel_pd:   resb 0x1000
kernel_pt:   resb 0x1000
