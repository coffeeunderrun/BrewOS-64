bits 16

global entry

extern drive, print, read_disk, reboot
extern load_file, open_volume

section .text

entry:
    ; Ensure execution at 0000:7C00
    jmp 0:stage1

stage1:
    cli
    cld

    mov ax, cs
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Top of stack starts at 0000:7C00
    xor ax, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Save boot drive number
    mov [drive], dl

    sti

    ; Load second half of stage 1 at 0000:7E00
    mov eax, 1     ; Start sector
    mov bx, ds     ; Destination segment
    mov cx, 1      ; Sector read count
    mov dx, 0x7E00 ; Destination offset
    call read_disk

    ; Load stage 2 boot loader at 0C40:0000
    call open_volume
    mov ax, filename        ; Filename
    mov cx, filename.length ; Filename length
    mov bx, 0xC40
    mov es, bx              ; Destination segment
    call load_file

    ; Check if file was found
    cmp eax, 0
    jz error_not_found

    ; Jump to stage 2 boot loader
    mov dl, [drive]
    jmp 0:0xC400

error_not_found:
    mov si, msg.not_found
    call print
    jmp reboot

filename: db "loader"
.length:  equ $ - filename

msg:
.not_found: db "Loader not found.", 13, 10, 0
