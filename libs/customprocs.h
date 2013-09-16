/*
Custom procedures, mainly related to character arrays.
Copyright (C) 2009  Philippe Proulx (philippe.proulx@polymtl.ca)

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

#ifndef _CUSTOMPROCS_H
#define _CUSTOMPROCS_H

#include <avr/io.h>

#define CP_HEX_CHARLIST		"0123456789abcdef" /* Hex chars */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * String length.
 *
 * @param str		ASCIIZ string
 * @return		String length
 */
uint16_t cp_strlen(const char* str);

/**
 * Unsigned integer to ASCIIZ string.
 *
 * @param buf		Buffer to be used
 * @param buf_len	Buffer length (be sure it's long enough)
 * @param val		Unsigned integer value
 * @param base		Base (between 2 and 16)
 * @return		ASCIIZ string (pointing inside given buffer)
 */
char* cp_utoa(char* buf, uint8_t buf_len, uint16_t val, uint8_t base);

/**
 * Signed integer to ASCIIZ string.
 *
 * @param buf		Buffer to be used
 * @param buf_len	Buffer length (be sure it's long enough)
 * @param val		Signed integer value
 * @param base		Base (between 2 and 16)
 * @return		ASCIIZ string (pointing inside given buffer)
 */
char* cp_itoa(char* buf, uint8_t buf_len, int16_t val, uint8_t base);

/**
 * Waits a variable number of milliseconds.
 *
 * @param ms	Number of milliseconds to wait
 */
void cp_wait_ms(uint16_t ms);

#ifdef __cplusplus
}
#endif

#endif /* _CUSTOMPROCS_H */
