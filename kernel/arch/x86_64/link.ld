OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_entry32)
VMA = 0xFFFFFFFF80000000;
SECTIONS
{
	. = 1M;
	_kernel_start = .;

	. ALIGN(4K) :
	{
		*(.multiboot .bootstrap)
	}

	. += VMA;
	.text ALIGN(4K) : AT(ADDR(.text) - VMA)
	{
		*(.text .rodata* .eh_frame)
	}

	.data ALIGN(4K) : AT(ADDR(.data) - VMA)
	{
		*(.data)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - VMA)
	{
		_kernel_bss = . - VMA;
		*(.bss)
	}

	. = ALIGN(4K);
	_kernel_end = . - VMA;

	/DISCARD/ :
	{
		*(.comment)
	}
}
