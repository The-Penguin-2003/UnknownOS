/* Unknown OS Serial I/O Driver */

#include <io.h>
#include <vga.h>
#include <serial.h>

int32_t init_serial()
{
	outb(PORT+1, 0x00);
	outb(PORT+3, 0x80);
	outb(PORT+0, 0x03);
	outb(PORT+1, 0x00);

	outb(PORT+3, 0x03);
	outb(PORT+2, 0xC7);
	outb(PORT+4, 0x0B);
	outb(PORT+4, 0x1E);
	outb(PORT+0, 0xAE);

	if (inb(PORT+0) != 0xAE)
		return 1;

	outb(PORT+4, 0x0F);
	puts("[SERIAL] Initialized serial I/O on port COM1\r\n");
	return 0;
}

int32_t is_transmit_empty()
{
	return inb(PORT+5) & 0x20;
}

void serial_putc(const int8_t c)
{
	while (is_transmit_empty() == 0);
	outb(PORT, c);
}

void serial_puts(const int8_t* str)
{
	uint32_t i = 0;
	serial_putc(str[0]);

	while (*str++)
		serial_putc(str[i]);
}

void serial_log(const int8_t* str, const int32_t err_level)
{
	switch (err_level)
	{
		case 0:
			serial_puts("[ERROR] ");
			break;
		case 1:
			serial_puts("[WARNING] ");
			break;
		case 2:
			serial_puts("[INFO] ");
			break;
		case 3:
			serial_puts("[DEBUG] ");
			break;
	}

	serial_puts(str);
}
