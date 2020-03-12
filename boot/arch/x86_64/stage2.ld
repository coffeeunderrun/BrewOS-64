OUTPUT_ARCH(i386:x86-64)
OUTPUT_FORMAT(binary)
ENTRY(stage2)
SECTIONS
{
	. = 0xC400;

	.text :
	{
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
