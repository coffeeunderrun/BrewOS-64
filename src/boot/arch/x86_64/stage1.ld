OUTPUT_ARCH(i386)
OUTPUT_FORMAT(binary)
ENTRY(entry)
SECTIONS
{
	. = 0x7C00;

	.text :
	{
		start = .;

		/* Stage 1 */
		*stage1*.o
		*bios*.o
		ASSERT(. - start <= 510, "Stage 1 is too large.");

		/* Boot Signature */
		. = 510;
		SHORT(0xAA55)

		/* Stage 1.5 */
		*(.text .text*)
		ASSERT(. - start <= 1024, "Stage 1.5 is too large.");
	}

	/DISCARD/ :
	{
		*(.comment)
	}
}
