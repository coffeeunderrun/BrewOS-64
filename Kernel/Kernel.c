volatile char * text_memory = (volatile char *)0xB8000;

void kernel_main()
{
	*text_memory = 'B';

	while (1);
}