bits 32

section .bootstrap

global _entry32
_entry32:
  cli
  cld

check_multiboot_provided:
  cmp eax, 0x36D76289
  je .passed
  hlt
.passed:
  mov [mb_ptr], ebx

check_longmode_available:
  mov eax, 0x80000001
  cpuid
  mov ebp, edx
  bt ebp, 29
  jc .passed
  hlt
.passed:

populate_pml4_table:
  mov edi, pml4
  mov eax, pml3
  or eax, 0x3
  stosd
  mov edi, pml4 + 4088
  stosd

populate_pml3_table:
  mov edi, pml3
  mov eax, 0x83
  stosd
  mov edi, pml3 + 4080
  stosd

  mov edi, pml3 + 4088
  add eax, 0x100000
  stosd

enable_paging:
  mov eax, pml4
  mov cr3, eax
  mov eax, cr4
  bts eax, 5
  bt ebp, 26
  jnc .no_global
  bts eax, 7
.no_global:
  mov cr4, eax

enable_longmode:
  mov ecx, 0xC0000080
  rdmsr
  bts eax, 8
  bt ebp, 20
  jnc .no_nx
  bts eax, 11
.no_nx:
  wrmsr
  mov eax, cr0
  bts eax, 31
  mov cr0, eax
  lgdt [gdt.low]
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

  mov rbx, [mb_ptr]
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
  mov rax, gdt.high
  add rax, 0xFFFFFFFF80000000
  lgdt [rax]

  mov rsp, stack.end
  push rbx
  jmp _kernel_main

section .bss

stack:
.start:
  resb 0x100000
.end:
