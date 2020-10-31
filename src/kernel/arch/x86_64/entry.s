bits 64

extern _kernel_bss, _kernel_bss_size, kmain

section .text

global entry
entry:
    ; Don't rely on the stage 2 loader GDT
    mov rax, qword gdt.ptr
    lidt [rax]

    mov rax, qword trampoline
    jmp gdt.kern_code:rax

trampoline:
    mov ax, gdt.kern_data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Save pointer to memory map
    mov rbp, rdi

    ; Zero-out BSS section
    xor rax, rax
    mov rcx, qword _kernel_bss_size
    mov rdi, qword _kernel_bss
    rep stosb

    ; Set up new stack
    mov rsp, qword kernel_stack_top

    ; Restore pointer to memory map
    mov rdi, rbp

    ; Jump to kernel
    jmp kmain

section .data

align 0x1000
global gdt.kern_code, gdt.user_code
gdt:
.null:       dq 0
.kern_code:  equ $ - gdt
             dq 0x00209A0000000000
.kern_data:  equ $ - gdt
             dq 0x0000920000000000
.user_code:  equ $ - gdt
             dq 0x0020FA0000000000
.user_data:  equ $ - gdt
             dq 0x0000F20000000000
.ptr:        dw $ - gdt - 1
             dq gdt

section .bss

align 0x1000
resb 0x2000
kernel_stack_top:
