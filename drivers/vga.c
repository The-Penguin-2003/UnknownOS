/* Unknown OS VGA Driver */

#include <sys.h>
#include <io.h>
#include <vga.h>

uint16_t* vga_memory;
int32_t attrib = 0x0F;
int32_t cursor_x = 0;
int32_t cursor_y = 0;

void scroll()
{
	unsigned blank, tmp;
	blank = 0x20 | (attrib << 8);

	if (cursor_y >= 25)
	{
		tmp = cursor_y - 25 + 1;
		memcpy(vga_memory, vga_memory + tmp * 80, (25 - tmp) * 80 * 2);

		memsetw(vga_memory + (25 - tmp) * 80, blank, 80);
		cursor_y = 25 - 1;
	}
}

void move_cursor()
{
	unsigned tmp;
	tmp = cursor_y * 80 + cursor_x;

	outb(0x3D4, 14);
	outb(0x3D5, tmp >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, tmp);
}

void clear()
{
	unsigned blank;
	int32_t i;
	blank = 0x20 | (attrib << 8);

	for (i = 0; i < 25; ++i)
		memsetw(vga_memory + i * 80, blank, 80);

	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

void putc(uint8_t c)
{
	uint16_t* where;
	unsigned att = attrib << 8;

	if (c == 0x08)
	{
		if (cursor_x != 0)
			cursor_x--;
	}
	else if (c == 0x09)
		cursor_x = (cursor_x + 8) & ~(8-1);
	else if (c == '\r')
		cursor_x = 0;
	else if (c == '\n')
	{
		cursor_x = 0;
		cursor_y++;
	}
	else if (c >= ' ')
	{
		where = vga_memory + (cursor_y * 80 + cursor_x);
		*where = c | att;
		cursor_x++;
	}

	if (cursor_x >= 80)
	{
		cursor_x = 0;
		cursor_y++;
	}

	scroll();
	move_cursor();
}

void puts(uint8_t* str)
{
	int32_t i;
	int32_t len = strlen(str);

	for (i = 0; i < len; ++i)
		putc(str[i]);
}

void set_text_color(uint8_t fg, uint8_t bg)
{
	attrib = (bg << 4) | (fg & 0x0F);
}

void init_vga()
{
	vga_memory = (uint16_t*)0xB8000;
	clear();
}
