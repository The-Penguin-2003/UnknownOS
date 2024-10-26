/* Unknown OS Kernel */

#include <vga.h>
#include <serial.h>

int32_t kernel_main()
{
	init_vga();
	puts("Welcome to the Unknown Operating System!\r\n");

	for (int32_t i = 0; i < 16; i++)
	{
		set_text_color(i, i);
		putc(' ');
	}

	set_text_color(15, 0);
	puts("\r\n");

	set_text_color(3, 0);
	init_serial();

	for (;;);
	return 0;
}
