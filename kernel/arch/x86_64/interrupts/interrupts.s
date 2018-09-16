bits 64

extern _irq_handler

PIC1_CTRL_PORT equ 0x20
PIC1_DATA_PORT equ 0x21
PIC2_CTRL_PORT equ 0xA0
PIC2_DATA_PORT equ 0xA1

PIC_END_IRQ equ 0x20

ICW1        equ 0x11
ICW2_MASTER equ 0x20
ICW2_SLAVE  equ 0x28
ICW3_MASTER equ 0x4
ICW3_SLAVE  equ 0x2
ICW4        equ 0x1

%ifndef pushaq
%macro pushaq 0
  push rax
  ; Save original stack pointer
  lea rax, [rsp + 8]      
  push rbx
  push rcx
  push rdx
  ; Push original stack pointer to stack
  push rax
  push rbp
  push rsi
  push rdi
%endmacro
%endif

%ifndef popaq
%macro popaq 0
  pop rdi
  pop rsi
  pop rbp
  ; Trash saved stack pointer
  add rsp, 8
  pop rdx
  pop rcx
  pop rbx
  pop rax
%endmacro
%endif

%macro IRQ_NO_ERROR 1-2 0
irq%1:
  cli
  push qword %2
  push qword %1
  pushaq
  call _irq_handler
  popaq
  add rsp, 16
  sti
  iretq
%endmacro

%macro IRQ_ERROR 1
irq%1:
  cli
  push qword %1
  pushaq
  call _irq_handler
  popaq
  add rsp, 16
  sti
  iretq
%endmacro

%macro IDT_ENTRIES 2
%assign i %1
%rep %2
  mov rax, irq%+ i
  call load_idt_entry
%assign i i + 1
%endrep
%endmacro

section .text

global _init_pics
_init_pics:
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

global _load_idt:
_load_idt:
  mov rdi, idt

  ; Entries 0 - 21
  IDT_ENTRIES 0, 22
  ; Skip entries 22 - 31
  add rdi, 1280
  ; Entries 32 - 47
  IDT_ENTRIES 32, 16
  ; Entry 128
  IDT_ENTRIES 128, 1

  mov rax, idt.ptr
  lidt [rax]
  sti
  ret

load_idt_entry:
  ; Base low
  stosw
  ;Selector
  mov ax, 0x8
  stosw
  ; Flags
  mov ax, 0x8E00
  stosw
  ; Base mid
  shr rax, 16
  stosw
  ; Base high
  shr rax, 16
  stosd
  ; Reserved
  xor rax, rax
  stosd
  ret

global _clear_hardware_irq
_clear_hardware_irq:
  cmp al, 8
  mov al, PIC_END_IRQ
  jge .pic2

  out PIC1_CTRL_PORT, al
  ret
.pic2:
  out PIC2_CTRL_PORT, al
  ret

; Software interrupts
IRQ_ERROR 8
IRQ_ERROR 10
IRQ_ERROR 11
IRQ_ERROR 12
IRQ_ERROR 13
IRQ_ERROR 14

IRQ_NO_ERROR 0
IRQ_NO_ERROR 1
IRQ_NO_ERROR 2
IRQ_NO_ERROR 3
IRQ_NO_ERROR 4
IRQ_NO_ERROR 5
IRQ_NO_ERROR 6
IRQ_NO_ERROR 7
IRQ_NO_ERROR 9
IRQ_NO_ERROR 15
IRQ_NO_ERROR 16
IRQ_NO_ERROR 17
IRQ_NO_ERROR 18
IRQ_NO_ERROR 19
IRQ_NO_ERROR 20
IRQ_NO_ERROR 21
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

align 4096

idt:
  times 4096 db 0
.ptr:
  dw $ - idt - 1
  dq idt
