OUTPUT_ARCH(i386:x86-64)
OUTPUT_FORMAT(elf64-x86-64)
ENTRY(main)
VMA = 0xFFFFFFFF80000000;
SECTIONS
{
	. = 0x100000 + VMA;

	.text ALIGN(4K) : AT(ADDR(.text) - VMA)
	{
		*(.text .text*)
		*(.rodata .rodata*)
	} = 0

	.data ALIGN(4K) : AT(ADDR(.data) - VMA)
	{
		*(.data)
	} = 0

	.bss ALIGN(4K) : AT(ADDR(.bss) - VMA)
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
