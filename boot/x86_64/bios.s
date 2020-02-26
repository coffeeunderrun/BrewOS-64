bits 16

global drive, print, read_disk, reboot

; Print [SI] text to screen
print:
    mov ah, 0xE
.next:
    lodsb
    test al, al ; Test for string terminator
    jz .done
    int 0x10
    jmp .next
.done:
    ret

; Read CX number of sectors into [BX:DX]
; Starting sector in EAX
; Assumes [drive] has been properly set
read_disk:
    push bx
    push dx
    push si

    mov [dap.start], eax  ; Start sector
    mov [dap.count], cx   ; Sector read count
    mov [dap.segment], bx ; Destination segment
    mov [dap.offset], dx  ; Destination offset

    mov dl, [drive] ; Boot drive number

    ; Check if extended read is supported
    mov ah, 0x41
    mov bx, 0x55AA
    int 0x13
    ; TODO: Implement LBA to CHS if extended read is not supported
    jc error_bad_disk

    ; Read from disk
    mov ah, 0x42
    mov si, dap
    int 0x13
    jc error_bad_disk

    pop si
    pop dx
    pop bx
    ret

reboot:
    cli
    mov si, msg.press_key
    call print
    xor ax, ax
    int 0x16  ; Wait for keypress
    int 0x19  ; Reboot

error_bad_disk:
    mov si, msg.bad_disk
    call print
    jmp reboot

msg:
.bad_disk:  db "Disk failure.", 13, 10, 0
.press_key: db "Press any key...", 13, 10, 0

drive: db 0

dap:      db 16, 0 ; Packet size
.count:   dw 0     ; Sectors to read
.offset:  dw 0     ; Destination offset
.segment: dw 0     ; Destination segment
.start:   dq 0     ; Start sector
