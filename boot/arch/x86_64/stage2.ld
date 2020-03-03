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
	} = 0

	.data :
	{
		*(.data)
	} = 0

	.bss :
	{
		*(.bss)
		*(COMMON)
	} = 0

	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame)
	}
}
