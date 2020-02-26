bits 16

extern drive, print, read_disk, reboot
extern load_file, open_volume

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

    push ds
    sti

    ; Load kernel at 2000:0000
    call open_volume
    mov ax, filename        ; Filename
    mov cx, filename.length ; Filename length
    xor dx, dx              ; File start block offset
    mov bx, 0x2000
    mov es, bx              ; Destination segment
    call load_file

    ; Check if file was found
    cmp eax, 0
    jz error_not_found
    push eax

    cli

    ; Enable A20
    mov ax, 0x2401
    int 0x15
    jc error_incompatible

    ; Get A20 status
    mov ax, 0x2402
    int 0x15
    jc error_incompatible

    ; Verify A20 is enabled
    cmp al, 0
    je error_incompatible

    ; Load GDT register
    lgdt [gdt.ptr]

    ; Enable protected mode
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp $ + 2

    ; Flat data descriptor
    mov bx, 0x8
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    ; Disable protected mode
    and al, 0xFE
    mov cr0, eax

    pop ecx           ; Number of bytes to copy
    mov esi, 0x20000  ; Read from 128 KiB location
    mov edi, 0x100000 ; Copy to 1 MiB location
    a32 rep movsb

    pop ds
    sti

    ; TODO: Enable long mode, parse ELF headers, and jump to kernel entry point
    jmp $

error_incompatible:
    mov si, msg.incompatible
    call print
    jmp reboot

error_not_found:
    mov si, msg.not_found
    call print
    jmp reboot

filename: db "brewkern"
.length:  equ $ - filename

msg:
.incompatible: db "Incompatible system.", 13, 10, 0
.not_found:    db "Kernel not found.", 13, 10, 0

gdt:
.null: dq 0
.data: dq 0xCF92000000FFFF
.ptr:  dw $ - gdt - 1
       dd gdt + 0x10000
