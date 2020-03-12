OUTPUT_ARCH(i386)
OUTPUT_FORMAT(binary)
ENTRY(entry)
SECTIONS
{
	. = 0x7C00;

	.text :
	{
		*stage1.s.o
		*bios.s.o
		. = 510;
		SHORT(0xAA55)
		*(.text .text*)
		*(.rodata .rodata*)
	}

	.data :
	{
		*(.data)
	}

	.bss :
	{
		*(.bss)
		*(COMMON)
	}

	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame)
	}
}
