/*
Custom procedures for AVR MCUs.
Copyright (C) 2011  Philippe Proulx (eeppeliteloop@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/io.h>
#include <util/delay.h>

#include "customprocs.h"

/**
 * Absolute (unsigned) integer to ASCIIZ string - for internal use only.
 *
 * @param buf		Buffer to be used
 * @param val		Value
 * @param base		Base (between 2 and 16)
 * @param init_i	Initial position inside buffer for the algorithm
 * @return		Position of string inside given buffer
 */
static uint8_t _cp_abs_ntoa(char* buf, uint16_t val, uint8_t base,
uint8_t init_i) {
	uint8_t i = init_i;
	if (val == 0) {
		buf[i] = '0';
		--i;
	}
	for(; val && i; --i, val /= base) {
		buf[i] = CP_HEX_CHARLIST[val % base];
	}
	
	return i;
}

uint16_t cp_strlen(const char* str) {
	uint16_t i = '\0';

	while (str[++i] != '\0');

	return i;
}

char* cp_utoa(char* buf, uint8_t buf_len, uint16_t val, uint8_t base) {
	uint8_t i = buf_len - 2;
	
	buf[buf_len - 1] = '\0';
	i = _cp_abs_ntoa(buf, val, base, i);
	
	return &buf[i + 1];
}

char* cp_itoa(char* buf, uint8_t buf_len, int16_t val, uint8_t base) {
	uint8_t i = buf_len - 2;
	uint8_t is_neg = (val < 0);
	uint16_t abs_val;
	
	buf[buf_len - 1] = '\0';
	abs_val = (uint16_t) (is_neg ? val * -1 : val);
	i = _cp_abs_ntoa(buf, abs_val, base, i);
	if (is_neg) {
		buf[i] = '-';
		--i;
	}

	return &buf[i + 1];
}

void cp_wait_ms(uint16_t ms) {
	while (ms--) {
		_delay_ms(1.0);
	}
}
