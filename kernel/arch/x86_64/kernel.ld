OUTPUT_ARCH(i386:x86-64)
OUTPUT_FORMAT(elf64-x86-64)
ENTRY(entry)
SECTIONS
{
	. = 0xFFFFFFFF80100000;

	.text ALIGN(4K) : AT(ADDR(.text) - 0xFFFFFFFF80000000)
	{
		kernel_start = .;
		*(.text .text*)
		*(.rodata .rodata*)
	} = 0

	.data ALIGN(4K) : AT(ADDR(.data) - 0xFFFFFFFF80000000)
	{
		kernel_data = .;
		*(.data)
	} = 0

	.bss ALIGN(4K) : AT(ADDR(.bss) - 0xFFFFFFFF80000000)
	{
		*(COMMON)
		*(.bss)
		kernel_end = .;
	} = 0

	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame)
	}
}
