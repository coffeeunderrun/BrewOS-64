OUTPUT_ARCH(i386:x86-64)
OUTPUT_FORMAT(elf64-x86-64)
ENTRY(entry)
SECTIONS
{
	. = 0xFFFFFFFF80100000;

	.text ALIGN(4K) : AT(ADDR(.text) - 0xFFFFFFFF80000000)
	{
		_kernel_start = .;
		*(.text .text*)
		*(.rodata .rodata*)
	}

	.data ALIGN(4K) : AT(ADDR(.data) - 0xFFFFFFFF80000000)
	{
		_kernel_data = .;
		*(.data)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - 0xFFFFFFFF80000000)
	{
		_kernel_bss = .;
		*(COMMON)
		*(.bss)
	}

	_kernel_bss_size = SIZEOF(.bss);

	_heap_start = .;
	. += 0x800000;
	_heap_end = .;

	_kernel_end = .;

	/DISCARD/ :
	{
		*(.comment)
	}
}
