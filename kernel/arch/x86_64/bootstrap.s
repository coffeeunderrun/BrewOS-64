bits 32

section .bootstrap

global _entry32
_entry32:
  cli
  cld

check_multiboot_provided:
  cmp eax, 0x36D76289
  je .multiboot_available
  hlt
.multiboot_available:
  mov [mb_ptr], ebx

check_longmode_available:
  mov eax, 0x80000001
  cpuid
  mov ebp, edx
  bt ebp, 29
  jc .longmode_available
  hlt
.longmode_available:

; The kernel VMA starts at the last 2GB of virtual address space
; This is why we populate the last entry of the PML4 and last 2 entries of the PML3

populate_pml4_table:
  mov edi, pml4
  mov eax, pml3
  or eax, 0x3
  ; Populate the first entry to cover where the kernel is stored in physical memory
  stosd
  ; 4088 is entry 511 (last entry)
  mov edi, pml4 + 4088
  ; Populate the last entry to cover the last 512GB of virtual address space
  ; This is a 1:1 mapping to the location of the kernel in physical memory
  stosd

populate_pml3_table:
  mov edi, pml3
  ; Map the first 1GB of physical memory
  mov eax, 0x83
  ; Populate the first entry to cover where the kernel is stored in physical memory
  stosd
  ; 4080 is entry 510 (2nd to last entry)
  mov edi, pml3 + 4080
  stosd

  ; We don't need to populate the 2nd entry because the bootstrap falls within the first 1GB
  ; We only need the bootstrap in the lower half until we jump to the higher half
  ; Once in the higher half, we can remove the first entry of the PML3 and PML4

  ; 4088 is entry 511 (last entry)
  mov edi, pml3 + 4088
  ; Map the second GB of physical memory
  add eax, 0x100000
  stosd

enable_paging:
  mov eax, pml4
  mov cr3, eax
  mov eax, cr4
  bts eax, 5
  bt ebp, 26
  jnc .global_pages_not_available
  bts eax, 7
.global_pages_not_available:
  mov cr4, eax

enable_longmode:
  mov ecx, 0xC0000080
  rdmsr
  bts eax, 8
  bt ebp, 20
  jnc .no_execute_not_available
  bts eax, 11
.no_execute_not_available:
  wrmsr
  mov eax, cr0
  bts eax, 31
  mov cr0, eax

  ; Load GDT for lower half
  lgdt [gdt.low]

  ; Jump to 64bit code in lower half
  jmp gdt.code:trampoline

align 4096

pml4:
  times 4096 db 0

pml3:
  times 4096 db 0

mb_ptr:
  dq 0

bits 64

trampoline:
  xor ax, ax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  ; Pass multiboot pointer to higher half
  mov rbx, [mb_ptr]

  ; Jump to 64bit code in higher half
  mov rax, entry64
  jmp rax

gdt:
.null:
  dq 0
.code: equ $ - gdt
  dd 0
  dd 0x00209800
.low:
  dw $ - gdt - 1
  dq gdt
.high:
  dw .end - gdt - 1
  dq gdt + 0xFFFFFFFF80000000
.end:

section .text

extern _kernel_main

entry64:
  ; Load GDT for higher half
  mov rax, gdt.high
  add rax, 0xFFFFFFFF80000000
  lgdt [rax]

  ; Set stack pointer (grows down from end to start)
  mov rsp, stack.end

  ; Pass multiboot pointer to kernel main
  push rbx

  ; Jump to kernel main
  jmp _kernel_main

section .bss

stack:
.start:
  resb 0x100000
.end:
