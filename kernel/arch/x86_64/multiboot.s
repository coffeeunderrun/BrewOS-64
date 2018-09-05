bits 32

extern _entry32
extern _kernel_start
extern _kernel_end
extern _kernel_bss

section .multiboot

MB_MAGIC    equ 0xE85250D6
MB_ARCH     equ 0
MB_LENGTH   equ (header_end - header_start)
MB_CHECKSUM equ (0x100000000 - (MB_MAGIC + MB_ARCH + MB_LENGTH))

align 8

header_start:
  dd MB_MAGIC
  dd MB_ARCH
  dd MB_LENGTH
  dd MB_CHECKSUM

tag_address:
  dw 2
  dw 0
  dd .end - tag_address
  dd header_start
  dd _kernel_start
  dd _kernel_bss
  dd _kernel_end
.end:

tag_entry:
  dw 3
  dw 0
  dd .end - tag_entry
  dd _entry32
.end:

header_end:
