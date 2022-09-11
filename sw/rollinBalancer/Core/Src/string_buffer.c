/**
 * Martin Egli
 * 2022-09-10
 * string buffer module
 */

// - includes ------------------------------------------------------------------
#include <stdint.h>
#include "usbd_cdc_if.h"
#include "string_buffer.h"

// - defines -------------------------------------------------------------------

// - private variables ---------------------------------------------------------
#define STRING_BUFFER_SIZE	(256)
static struct {
	uint8_t data[STRING_BUFFER_SIZE];
	uint16_t wr_idx;
} string_buffer_container;

void string_buffer_new(void) {
	string_buffer_container.wr_idx = 0;
}

void string_buffer_send_usb(void) {
	if(string_buffer_container.wr_idx == 0) {
		// nothing to send
		return;
	}
	if(CDC_Transmit_FS(string_buffer_container.data, string_buffer_container.wr_idx) == 0) {
		// OK
		asm("NOP");
	}
}


uint32_t string_buffer_append_char(char c) {
	if(string_buffer_container.wr_idx < (STRING_BUFFER_SIZE-2)) {
		string_buffer_container.data[string_buffer_container.wr_idx++] = c;
		return 1;
	}
	return 0;
}

void string_buffer_append_string(char *str) {
	char c;
	// copy string
	while(1) {
		c = *str++;
		if(c == '\0') {
			// found end, do not copy
			break;
		}
		if(string_buffer_append_char(c) == 0) {
			// buffer full, stop
			break;
		}
	}
}

void string_buffer_append_int16(int16_t value) {
	uint16_t digit_pos, use_char;
	int16_t digits[] = {10000, 1000, 100, 10, 1};
	uint8_t digit_value;
	use_char = 0;
	// sign
	if(value < 0) {
		value *= -1;
		if(string_buffer_append_char('-') == 0) {
			// buffer full, stop
			return;
		}
	}
	if(value == 0) {
		// only append '0', and done here
		string_buffer_append_char('0');
		return;
	}
	for(digit_pos = 0; digit_pos < (sizeof(digits)/sizeof(digits[0])); digit_pos++) {
		digit_value = (uint8_t)(value/digits[digit_pos]);
		if((digit_value != 0) || (use_char != 0)) {
			use_char = 1;
			if(string_buffer_append_char('0'+digit_value) == 0) {
				// buffer full, stop
				return;
			}
			value -= digit_value * digits[digit_pos];
		}
	}
}

void string_buffer_append_int32(int32_t value) {
	uint16_t digit_pos, use_char;
	int32_t digits[] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
	uint8_t digit_value;
	use_char = 0;
	// sign
	if(value < 0) {
		value *= -1;
		if(string_buffer_append_char('-') == 0) {
			// buffer full, stop
			return;
		}
	}
	if(value == 0) {
		// only append '0', and done here
		string_buffer_append_char('0');
		return;
	}
	for(digit_pos = 0; digit_pos < (sizeof(digits)/sizeof(digits[0])); digit_pos++) {
		digit_value = (uint8_t)(value/digits[digit_pos]);
		if((digit_value != 0) || (use_char != 0)) {
			use_char = 1;
			if(string_buffer_append_char('0'+digit_value) == 0) {
				// buffer full, stop
				return;
			}
			value -= digit_value * digits[digit_pos];
		}
	}
}

void string_buffer_append_float(float f, uint32_t nb_dec_places) {
	int32_t x;

	x = (int32_t)f;
	string_buffer_append_int32(x);

	if(nb_dec_places == 0) {
		// do not add any decimal places
		return;

	}
	if(nb_dec_places >= 9) {
		// max is 9
		nb_dec_places = 9;
	}

	f -= (float)x;
	if(f < 0.0f) {
		f *= -1;
	}
	string_buffer_append_char('.');
	// eval nb leading '0' of part after '.'
	for(uint16_t n = 0; n < nb_dec_places; n++) {
		f *= 10;
		x = (uint8_t)f;
		f -= (float)x;
		string_buffer_append_char('0'+x);
	}
}
