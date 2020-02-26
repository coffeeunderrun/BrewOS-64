OUTPUT_ARCH(i386:x86-64)
OUTPUT_FORMAT(elf64-x86-64)
ENTRY(main)
SECTIONS
{
	/* Kernel lives in -2GB */
	. = 0xFFFFFFFF80000000;

	.text : ALIGN(4K) { *(.text .text*) *(.rodata .rodata*) } = 0

	.data : ALIGN(4K) { *(.data) } = 0

	.bss : ALIGN(4K) { *(.bss) *(COMMON) } = 0

	/DISCARD/ : { *(.comment) *(.eh_frame) }
}
