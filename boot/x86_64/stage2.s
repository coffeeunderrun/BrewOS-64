bits 16

extern drive, print, read_disk, reboot
extern load_file, open_volume

pml4 equ 0x1000
pml3 equ 0x2000

section .text

global stage2
stage2:
    cli
    cld

    ; Assume stage 1 already set up stack
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Save boot drive number
    mov [drive], dl

    ; Load kernel at 2000:0000
    call open_volume
    mov ax, filename        ; Filename
    mov cx, filename.length ; Filename length
    mov bx, 0x2000
    mov es, bx              ; Destination segment
    call load_file
    mov [filesize], eax

    ; Check if file was found
    cmp eax, 0
    jz error_not_found

    ; Reset ES
    xor ax, ax
    mov es, ax

    ; Enable A20
    mov ax, 0x2401
    int 0x15

    ; Disable IRQs
    mov al, 0xFF
    out 0xA1, al
    out 0x21, al

    ; Load IDT register
    lidt [idt]

    ; Load GDT register
    lgdt [gdt.low]

    ; Zero out PML4 and PML3
    xor eax, eax
    mov ecx, 0x1000
    mov edi, pml4
    rep stosd

    ; PML4
    mov eax, pml3
    or eax, 0x3
    mov [pml4], eax        ; First 512 GiB
    mov [pml4 + 4088], eax ; Last 512 GiB

    ; PML3
    mov eax, 0x83
    mov [pml3], eax        ; First 1 GiB
    mov [pml3 + 4080], eax ; Second to last 1 GiB
    add eax, 0x100000
    mov [pml3 + 4088], eax ; Last 1 GiB

    ; Point CR3 at the PML4
    mov edx, pml4
    mov cr3, edx

    ; Set PAE and PGE bit
    mov eax, cr4
    or eax, 0xA0
    mov cr4, eax

    ; Set LME and NXE bit
    mov ecx, 0xC0000080
    rdmsr
    or eax, 0x100
    wrmsr

    ; Enable long mode
    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax

    mov ax, gdt.data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Jump to long mode
    jmp gdt.code:trampoline

error_not_found:
    mov si, msg.not_found
    call print
    jmp reboot

filesize: dq 0
filename: db "brewkern"
.length:  equ $ - filename

msg:
.not_found: db "Kernel not found.", 13, 10, 0

bits 64

trampoline:
    ; Parse ELF headers and load segments into proper locations
    mov rax, [0x20020]       ; Program header offset
    add rax, 0x20000         ; Where kernel was originally loaded
    xor rcx, rcx
    mov cx, [0x20038]        ; Program header count
.next:
    push rcx
    mov rbx, [rax + 0x8]     ; Segment offset from start of file
    mov rcx, [rax + 0x20]    ; Segment size in bytes to copy
    mov rdi, [rax + 0x18]    ; Physical memory address destination
    lea rsi, [rbx + 0x20000] ; Source location offset
    rep movsb
    pop rcx
    add ax, [0x20036]        ; Point to net program header
    loop .next               ; Repeat if any program headers remaining

    ; Load GDT register again using higher half pointer
    mov rax, 0xFFFFFFFF80000000
    add rax, gdt.high
    lgdt [rax]

    ; Jump to kernel in higher half
    mov rax, [0x20018] ; Entry point
    jmp rax

align 8
idt:
.length: dw 0
.base:   dd 0

align 8
gdt:
.null: dq 0
.code: equ $ - gdt
       dq 0x00209A0000000000
.data: equ $ - gdt
       dq 0x0000920000000000
.low:  dw $ - gdt - 1
       dq gdt
.high: dw .end - gdt - 1
       dq gdt + 0xFFFFFFFF80000000
.end:
