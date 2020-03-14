bits 16

global stage2

extern drive, print, read_disk, reboot
extern load_file, open_volume

kb equ 0x20000            ; Kernel buffer
mm equ 0x70000            ; Memory map
p4 equ 0x1000             ; Temporary PML4
p3 equ 0x2000             ; Temporary PDP
vo equ 0xFFFFFFFF80000000 ; Kernel virtual higher half offset

section .text

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
    mov bx, kb >> 4
    mov es, bx              ; Destination segment
    call load_file
    mov [filesize], eax

    ; Check if file was found
    cmp eax, 0
    jz error_not_found

    ; Get memory map
    mov bx, mm >> 4
    mov es, bx                 ; Destination segment
    xor ebx, ebx
    xor edi, edi
    mov edx, 0x534D4150        ; Magic number
.next_entry:
    mov [es:edi + 20], dword 1 ; ACPI compatibility
    mov eax, 0xE820
    mov ecx, 0x18              ; 24 byte entry
    int 0x15
    jc .last_entry
    add edi, 0x18              ; Point to next entry
    test ebx, ebx
    jnz .next_entry
.last_entry:

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

    ; Zero out page tables
    xor eax, eax
    mov ecx, 0x1000
    mov edi, p4
    rep stosd

    ; These page tables are temporary and will be trashed by the kernel later
    ; PML4 table
    mov eax, p3
    or eax, 0x3             ; Present and writable
    mov [p4], eax           ; Identity map first 512 GiB
    mov [p4 + 511 * 8], eax ; Map last 512 GiB to first 512 GiB
    mov eax, p4
    or eax, 0x3
    mov [p4 + 510 * 8], eax ; Recursively map second to last entry to PML4 table

    ; Page directory pointer table
    mov eax, 0x83           ; Present and writable
    mov [p3], eax           ; Identity map first 1 GiB
    mov [p3 + 510 * 8], eax ; Map second to last 1 GiB to first 1 GiB

    ; Point CR3 at the PML4
    mov edx, p4
    mov cr3, edx

    ; Set PAE and PGE bit
    mov eax, cr4
    or eax, 0xA0
    mov cr4, eax

    ; Set LME and NXE bit
    mov ecx, 0xC0000080
    rdmsr
    or eax, 0x900
    wrmsr

    ; Set PG, WP, and PE bit
    mov eax, cr0
    or eax, 0x80010001
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
    ; Load GDT register again using higher half pointer
    mov rax, qword gdt.high + vo
    lgdt [rax]

    ; Load ELF program segments into proper locations
    mov rax, [kb + 0x20]  ; Program header offset
    add rax, kb           ; Where kernel was originally loaded
    xor rcx, rcx
    mov cx, [kb + 0x38]   ; Program header count
.next_segment:
    push rcx
    mov rbx, [rax + 0x8]  ; Segment offset from start of file
    mov rcx, [rax + 0x28] ; Segment size in bytes to copy
    mov rdi, [rax + 0x18] ; Physical memory address destination
    lea rsi, [rbx + kb]   ; Source location offset
    rep movsb
    pop rcx
    add ax, [kb + 0x36]   ; Point to next program header
    loop .next_segment    ; Repeat if any program headers remaining

    ; TODO: Pass memory map entry count to kernel
    ; Pass memory map pointer to kernel
    mov rdi, qword mm + vo

    ; Jump to entry point in higher half
    jmp qword [kb + 0x18]

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
       dq gdt + vo
.end:
