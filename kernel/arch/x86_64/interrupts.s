bits 64

global clear_irq, init_pics, load_idt

extern irq_handler

PIC1_CTRL_PORT equ 0x20
PIC1_DATA_PORT equ 0x21
PIC2_CTRL_PORT equ 0xA0
PIC2_DATA_PORT equ 0xA1
PIC_END_IRQ    equ 0x20
ICW1           equ 0x11
ICW2_MASTER    equ 0x20
ICW2_SLAVE     equ 0x28
ICW3_MASTER    equ 0x4
ICW3_SLAVE     equ 0x2
ICW4           equ 0x1

%macro IRQ_NO_ERROR 1-2 0
irq%1:
    push %2 ; Error Low
    push %1 ; Vector Low
    jmp irq_stub
%endmacro

%macro IRQ_ERROR 1
irq%1:
    push %1 ; Vector
    jmp irq_stub
%endmacro

%macro LOAD_IDT_ENTRY 1-4 0x8, 0x8E, 0
    mov rax, qword irq%1           ; Address of ISR
    mov cx, %2                     ; Segment selector
    mov dh, %3                     ; Flags
    mov dl, %4                     ; IST offset
    mov rdi, qword idt + (%1 * 16) ; Point to entry in IDT
    call load_idt_entry
%endmacro

section .text

; Tell the PIC that the interrupt has been handled
; Interrupt vector to be clear in AL
; Vectors 32-39, originally 0-7, are PIC1
; Vectors 40-47, originally 8-15, are PIC2
clear_irq:
    cmp al, 32
    jb .done
    cmp al, 47
    ja .done
    cmp al, 40
    mov al, PIC_END_IRQ
    mov dx, PIC1_CTRL_PORT
    jb .1
    mov dx, PIC2_CTRL_PORT
.1: out dx, al
.done:
    ret

; Enable PICs and remap hardware IRQ vectors
init_pics:
    ; Initialize programmable interrupt controllers
    mov al, ICW1
    out PIC1_CTRL_PORT, al
    out PIC2_CTRL_PORT, al

    ; Remap PICs as first 32 interrupts are reserved
    mov al, ICW2_MASTER
    out PIC1_DATA_PORT, al
    mov al, ICW2_SLAVE
    out PIC2_DATA_PORT, al

    ; Cascading
    mov al, ICW3_MASTER
    out PIC1_DATA_PORT, al
    mov al, ICW3_SLAVE
    out PIC2_DATA_PORT, al

    ; Environment info
    mov al, ICW4
    out PIC1_DATA_PORT, al
    out PIC2_DATA_PORT, al

    ; Mask interrupts
    xor al, al
    out PIC1_DATA_PORT, al
    out PIC2_DATA_PORT, al
    ret

load_idt:
    ; Zero out IDT
    xor rax, rax
    mov rcx, 0x200
    mov rdi, qword idt
    rep stosq

    LOAD_IDT_ENTRY 0  ; Divide by zero error
    LOAD_IDT_ENTRY 1  ; Debug
    LOAD_IDT_ENTRY 2  ; NMI
    LOAD_IDT_ENTRY 3  ; Breakpoint
    LOAD_IDT_ENTRY 4  ; Overflow
    LOAD_IDT_ENTRY 5  ; Bound range
    LOAD_IDT_ENTRY 6  ; Invalid opcode
    LOAD_IDT_ENTRY 7  ; Device not available
    LOAD_IDT_ENTRY 8  ; Double fault
    LOAD_IDT_ENTRY 9  ; Coprocessor segment overrun
    LOAD_IDT_ENTRY 10 ; Invalid TSS
    LOAD_IDT_ENTRY 11 ; Segment not present
    LOAD_IDT_ENTRY 12 ; Stack
    LOAD_IDT_ENTRY 13 ; General protection
    LOAD_IDT_ENTRY 14 ; Page fault

    ; Vector 15 is reserved

    LOAD_IDT_ENTRY 16 ; x87 floating point
    LOAD_IDT_ENTRY 17 ; Alignment check
    LOAD_IDT_ENTRY 18 ; Machine check
    LOAD_IDT_ENTRY 19 ; SIMD floating point
    LOAD_IDT_ENTRY 20 ; Virtualization

    ; Vectors 21 - 29 are reserved

    LOAD_IDT_ENTRY 30 ; Security

    ; Vector 31 is reserved

    LOAD_IDT_ENTRY 32 ; PIT
    LOAD_IDT_ENTRY 33 ; Keyboard
    LOAD_IDT_ENTRY 34 ; Cascade
    LOAD_IDT_ENTRY 35 ; COM2
    LOAD_IDT_ENTRY 36 ; COM1
    LOAD_IDT_ENTRY 37 ; LPT2
    LOAD_IDT_ENTRY 38 ; Floppy disk
    LOAD_IDT_ENTRY 39 ; LPT1
    LOAD_IDT_ENTRY 40 ; RTC
    LOAD_IDT_ENTRY 41 ; Available
    LOAD_IDT_ENTRY 42 ; Available
    LOAD_IDT_ENTRY 43 ; Available
    LOAD_IDT_ENTRY 44 ; Mouse
    LOAD_IDT_ENTRY 45 ; FPU
    LOAD_IDT_ENTRY 46 ; Primary hard disk
    LOAD_IDT_ENTRY 47 ; Secondary hard disk

    ; Vectors 48 - 127 are unused

    LOAD_IDT_ENTRY 128, 0x8, 0xEE ; Syscall from user land

    ; Vectors 129 - 255 are unused

    mov rax, qword idt_ptr
    lidt [rax]
    sti
    ret

; Add descriptor entry to IDT
; Address of ISR in RAX
; Segment selector in CX
; IST offset in DL
; Flags in DH
load_idt_entry:
    ; Offset low
    stosw

    ; Selector
    mov ax, 0x8
    stosw

    ; IST offset and flags
    mov ax, dx
    stosw

    ; Offset mid
    shr rax, 16
    stosw

    ; Offset high
    shr rax, 16
    stosd

    ; Reserved
    xor rax, rax
    stosd
    ret

; Push registers onto stack and call the handler
irq_stub:
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rsi
    push rdi
    push rbp
    push rdx
    push rcx
    push rbx
    push rax

    ; Pass register structure pointer to IRQ handler
    mov rdi, rsp
    call irq_handler

    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rbp
    pop rdi
    pop rsi
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15

    ; Remove vector and error from stack
    add rsp, 16
    iretq

; Software interrupts
IRQ_ERROR 8
IRQ_ERROR 10
IRQ_ERROR 11
IRQ_ERROR 12
IRQ_ERROR 13
IRQ_ERROR 14
IRQ_ERROR 17
IRQ_ERROR 30

IRQ_NO_ERROR 0
IRQ_NO_ERROR 1
IRQ_NO_ERROR 2
IRQ_NO_ERROR 3
IRQ_NO_ERROR 4
IRQ_NO_ERROR 5
IRQ_NO_ERROR 6
IRQ_NO_ERROR 7
IRQ_NO_ERROR 9
IRQ_NO_ERROR 16
IRQ_NO_ERROR 18
IRQ_NO_ERROR 19
IRQ_NO_ERROR 20
IRQ_NO_ERROR 128

; Hardware interrupts
IRQ_NO_ERROR 32, 0
IRQ_NO_ERROR 33, 1
IRQ_NO_ERROR 34, 2
IRQ_NO_ERROR 35, 3
IRQ_NO_ERROR 36, 4
IRQ_NO_ERROR 37, 5
IRQ_NO_ERROR 38, 6
IRQ_NO_ERROR 39, 7
IRQ_NO_ERROR 40, 8
IRQ_NO_ERROR 41, 9
IRQ_NO_ERROR 42, 10
IRQ_NO_ERROR 43, 11
IRQ_NO_ERROR 44, 12
IRQ_NO_ERROR 45, 13
IRQ_NO_ERROR 46, 14
IRQ_NO_ERROR 47, 15

section .data

align 8
idt_ptr: dw 0xFFF ; Limit
         dq idt   ; Offset

section .bss

align 0x1000
idt: resb 0x1000
