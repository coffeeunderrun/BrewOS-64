bits 64

global flush_page, flush_tlb, load_pml4

section .text

flush_page:
    invlpg [rdi]
    ret

flush_tlb:
    mov rax, cr3
    mov cr3, rax
    ret

load_pml4:
    mov cr3, rdi
    ret
