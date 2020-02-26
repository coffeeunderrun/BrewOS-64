bits 16

global load_file, open_volume

extern print, read_disk, reboot

sb equ 0x8000 ; Super block
bb equ 0xA000 ; Block buffer

; Searches root directory and loads file at ES:0000
; Filename pointer in AX and filename length in CX
; Returns bytes read in EAX or zero if file not found
load_file:
    push bp
    mov bp, sp
    sub sp, 6 ; Make room for local variables

    mov [bp - 2], ax ; Filename
    mov [bp - 4], cx ; Filename length

    ; Load root directory at ES:0000
    mov eax, 2       ; Inode 2
    call read_inode
    mov [bp - 6], ax ; Root directory size in bytes

    ; Search root directory for filename
    xor di, di
.check_entry:
    mov cx, [bp - 4]    ; Requested filename length
    cmp cl, [es:di + 6] ; Entry filename length
    jne .next_entry     ; Move on if the counts do not match
    push di             ; Save start of directory entry
    add di, 8           ; Entry filename at ES:DI
    mov si, [bp - 2]    ; Requested filename at DS:SI
    rep cmpsb           ; Compare filenames
    pop di              ; Restore start of directory entry
    jz .found
.next_entry:
    mov ax, [es:di + 4] ; Get total entry size
    add di, ax          ; Point to next entry
    cmp di, [bp - 6]    ; Check if we reached the end of the root directory
    jl .check_entry
    xor eax, eax        ; File not found
    jmp .done
.found:
    mov eax, [es:di]    ; File inode
    call read_inode
.done:
    leave
    ret

; Prepares boot drive volume for EXT2 functions
open_volume:
    ; Load super block
    mov eax, 2 ; Super block always starts at third sector
    mov bx, ds ; Destination segment
    mov cx, 2  ; Super block is always 1 KiB
    mov dx, sb ; Destination offset
    call read_disk

    ; Verify EXT2 signature
    mov ax, [sb + 56]
    cmp ax, 0xEF53
    jnz error_bad_fsys

    ; Calculate block size in bytes
    mov cx, [sb + 24] ; Log block size
    mov ax, 0x400     ; 1024 bytes
    shl ax, cl        ; Left-shift by log block size
    mov [bs], ax

    ; Calculate sectors per block
    inc cx    ; 512 bytes per sector
    shl cx, 1 ; Always atleast 2 sectors per block
    mov [spb], cx

    ; Calculate inodes per block
    xor dx, dx
    div word [sb + 88] ; Block size / inode size
    mov [ipb], ax

    ; Calculate block group descriptors per block
    mov ax, [bs]
    shr ax, 5 ; Block size / 32
    mov [dpb], ax
    ret

; Read block into [BX:DX]
; Block number in EAX
read_block:
    push cx
    push dx         ; Multiplying destroys DX value
    mul dword [spb] ; Start sector is block number times sectors per block
    pop dx
    mov cx, [spb]   ; Sectors per block
    call read_disk
    pop cx
    ret

; Load up to 268 KiB of inode stating at ES:0000
; ES will increment every 64 KiB read
; Inode number in EAX
; out EAX : Bytes read
read_inode:
    push bp
    mov bp, sp
    sub sp, 8 ; Make room for local variables
    push es

    dec eax           ; Inode is one-indexed
    mov [bp - 4], eax ; Inode number

    ; Calculate block group number
    xor edx, edx
    div dword [sb + 40] ; Divide inode number by inodes per group

    ; Calculate BGDT block offset
    xor edx, edx
    div dword [dpb] ; Divide block group number by descriptors per block
    mov bx, ax

    ; Calculate BGDT index
    shl dx, 5 ; Multiply remainder by 32
    mov di, dx

    ; Calculate BGDT start block
    xor dx, dx
    mov eax, 0x400    ; 1024 bytes
    div word [bs]     ; Divide by block size
    add ax, [sb + 20] ; Add super block number
    add ax, bx        ; Add block offset

    ; Load block group descriptor table
    mov bx, ds ; Destination segment
    mov dx, bb ; Destination offset
    call read_block

    ; Get inode table block id
    xor edx, edx
    mov eax, [bp - 4]      ; Inode number
    div dword [ipb]        ; Divide by inodes per block
    push edx               ; Remainder is inode table block offset
    add eax, [di + bb + 8] ; Add inode table start block id

    ; Load inode table
    mov bx, ds ; Destination segment
    mov dx, bb ; Destination offset
    call read_block

    ; Get inode structure pointer
    pop eax             ; Fetch inode table block offset
    mul dword [sb + 88] ; Multiply by inode structure size
    mov di, ax

    ; Calculate number of blocks to read
    xor edx, edx
    mov eax, [di + bb + 4] ; Inode size in bytes
    mov [bp - 8], eax
    add eax, [bs]          ; Round size up to nearest multiple of block size
    dec eax
    div dword [bs]         ; Divide by block size
    mov dx, ax

    mov ax, 12
    mov cx, dx
    cmp cx, ax
    cmova cx, ax ; Get smaller value between block count and 12

    sub dx, cx   ; Subtract direct block count from total
    push dx      ; Save block count remainder

    ; Load direct blocks
    xor dx, dx             ; Destination offset
    lea si, [di + bb + 40] ; Point to direct block array
    call read_direct_blocks

    pop cx     ; Block count remainder
    jcxz .done ; Are there any blocks left to read?

    ; Load singly indirect blocks
    lea si, [di + bb + 88] ; Point to singly indirect block array
    call read_singly_indirect_blocks
.done:
    mov eax, [bp - 8] ; Fetch total inode size in bytes
    pop es
    leave
    ret

; Load CX number of blocks at ES:DX using direct pointers from DS:SI
read_direct_blocks:
    mov bx, es   ; Destination segment
.next:
    lodsd        ; Get next block id from direct blocks array
    cmp eax, 0
    jz .1
    call read_block
.1: add dx, [bs] ; Advance destination pointer by block size
    jnz .2       ; Increase ES if DX reaches 64 KiB boundary
    add bx, 0x1000
    mov es, bx
.2: loop .next   ; Read next block
.done:
    ret

read_singly_indirect_blocks:
    push di
    push dx

    lodsd             ; Get singly indirect block number
    mov bx, ds        ; Destination segment
    mov di, [bs]      ; Don't overwrite inode table
    lea dx, [di + bb] ; Destination offset
    call read_block
    pop dx

    mov ax, [bs] ; Get block size in bytes
    shr ax, 2    ; Divide by four the get max number of block numbers
    cmp ax, cx
    cmova ax, cx ; Get smaller value
    sub cx, ax   ; Remove smaller value from total for remainder after reading direct blocks
    push cx

    mov cx, ax        ; Block read count
    lea si, [di + bb] ; Point at first block number
    call read_direct_blocks
    pop cx            ; Fetch block count
    sub cx, ax        ; Subtract number of blocks read

    pop di
    ret

error_bad_fsys:
    mov si, msg.bad_fsys
    call print
    jmp reboot

msg:
.bad_fsys: db "Invalid filesystem.", 13, 10, 0

bs:  dd 0 ; Block size
spb: dd 0 ; Sectors per block
ipb: dd 0 ; Inodes per block
dpb: dd 0 ; Descriptors per block
