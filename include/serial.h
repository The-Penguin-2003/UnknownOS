#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <types.h>

#define PORT 0x3F8

int32_t init_serial();
int32_t is_transmit_empty();
void serial_putc(const int8_t c);
void serial_puts(const int8_t* str);
void serial_log(const int8_t* str, const int32_t err_level);

#endif /* __SERIAL_H__ */
