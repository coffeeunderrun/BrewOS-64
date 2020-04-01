bits 64

section .text

global init_fpu
init_fpu:
    mov rax, 1
    cpuid         ; Get CPU features
    mov rax, cr4
    or rax, 0x600 ; Set OSFXSR and OSXMMEXCPT
    bt rcx, 26    ; Is OSXSAVE supported?
    jnc .1
    bts rax, 18   ; Set OSXSAVE
.1: mov cr4, rax
    mov rax, cr0
    or rax, 0x22  ; Set MP and NE
    btr rax, 2    ; Clear EM
    mov cr0, rax
    fninit        ; Initialize FPU
    ret

global flush_page
flush_page:
    invlpg [rdi]
    ret

global flush_tlb
flush_tlb:
    mov rax, cr3
    mov cr3, rax
    ret

global load_pml4
load_pml4:
    mov cr3, rdi
    ret
