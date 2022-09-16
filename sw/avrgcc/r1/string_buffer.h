/**
 * Martin Egli
 * 2022-09-10
 * string buffer module
 */

#ifndef _STRING_BUFFER_H_
#define _STRING_BUFFER_H_

// - includes ------------------------------------------------------------------
#include <stdint.h>

// - defines -------------------------------------------------------------------
#define SEP_SERIAL_PLOTTER ' ' // better serial plotter uses ' ' (space)
#define SEP SEP_SERIAL_PLOTTER

void string_buffer_new(void);
void string_buffer_send_uart(void);
uint32_t string_buffer_append_char(char c);
#define string_buffer_append_separator() string_buffer_append_char(SEP)
void string_buffer_append_string(char *str);
void string_buffer_append_int8(int8_t value);
void string_buffer_append_uint8(uint8_t value);
void string_buffer_append_int16(int16_t value);
//void string_buffer_append_int32(int32_t value);
void string_buffer_append_float(float f, uint32_t nb_dec_places);

#endif // _STRING_BUFFER_H_