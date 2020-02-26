OUTPUT_ARCH(i386)
OUTPUT_FORMAT(binary)
ENTRY(entry)
SECTIONS
{
	/* BIOS loads boot sector at 7C00 */
	. = 0x7C00;

	.text : { *stage1.s.o *bios.s.o ; . = 510 ; SHORT(0xAA55) *(.text .text*) *(.rodata .rodata*) } = 0

	.data : { *(.data) } = 0

	.bss : { *(.bss) *(COMMON) } = 0

	/DISCARD/ : { *(.comment) *(.eh_frame) }
}
