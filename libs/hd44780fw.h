/*
C framework atop the HD44780 device driver for AVR MCUs.
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

#ifndef _HD44780FW_H
#define _HD44780FW_H

#include <avr/io.h>

#include "hd44780_low.h"

/* Default parameters: */
#define HD44780FW_DEF_REM_CHAR	' '	/* Default remove character */
#define HD44780FW_DEF_NB_BASE	10	/* Default base when printing numbers */
#define HD44780FW_DEF_BLINK_ST	HD44780_L_DISP_B_OFF	/* Def. blink state */
#define HD44780FW_DEF_CUR_ST	HD44780_L_DISP_C_OFF	/* Def. cursor state */

/* Defined argument values: */
#define HD44780FW_WR_CLEAR_BEFORE	1	/* Clear before writing */
#define HD44780FW_WR_NO_CLEAR_BEFORE	0	/* No clear before writing */
#define HD44780FW_BLINK_ON	HD44780_L_DISP_B_ON	/* Enable blink */
#define HD44780FW_BLINK_OFF	HD44780_L_DISP_B_OFF	/* Disable blink */
#define HD44780FW_CURSOR_ON	HD44780_L_DISP_C_ON	/* Enable cursor */
#define HD44780FW_CURSOR_OFF	HD44780_L_DISP_C_OFF	/* Disable cursor */
#define HD44780FW_WR_NO_CLEAR_BEFORE	0	/* No clear before writing */
#define HD44780FW_WR_CLEAR_BEFORE	1	/* Clear before writing */
#define HD44780FW_WR_NO_CLEAR_BEFORE	0	/* No clear before writing */

/* Local buffer size: */
#define HD44780FW_BUF_SIZE	16

#ifdef __cplusplus
extern "C" {
#endif

/* HD44780 framework configuration structure: */
struct hd44780fw_conf {
	uint8_t total_chars;		/* Total chars on LCD display */
	uint8_t half_chars;		/* Half total chars on LCD display */
	uint8_t font;			/* Device character font */
	uint8_t lines;			/* Number of lines on device */
	uint8_t blink_en;		/* Blink state */
	uint8_t cur_en;			/* Cursor state */
	uint8_t last_index;		/* Last write index */
	uint8_t last_bc_index;		/* Last blink/cursor index */
	char buf [HD44780FW_BUF_SIZE];		/* Internal buffer */
	const struct hd44780_l_conf* low_conf;	/* Low-level driver conf. */
};

/**
 * Initializes the framework and the device.
 *
 * @param conf		HD44780 framework configuration
 */
void hd44780fw_init(struct hd44780fw_conf* conf);

/**
 * Puts a single character at current v. cursor position.
 *
 * @param conf		HD44780 framework configuration
 * @param ch		Character to put
 */
void hd44780fw_put(struct hd44780fw_conf* conf, char ch);

/**
 * Writes a string at given position.
 *
 * @param conf 		HD44780 framework configuration
 * @param msg		String to be written
 * @param index		Position of first character on device
 * @param cb		Clear display before writing string
 */
void hd44780fw_write(struct hd44780fw_conf* conf, const char* msg,
	uint8_t index, uint8_t cb);

/**
 * Clears display.
 *
 * @param conf 		HD44780 framework configuration
 */
void hd44780fw_clear(struct hd44780fw_conf* conf);

/**
 * Enables/disables blink.
 *
 * @param conf 		HD44780 framework configuration
 * @param state		Blink state
 */
void hd44780fw_en_blink(struct hd44780fw_conf* conf, uint8_t state);

/**
 * Enables/disables cursor.
 *
 * @param conf 		HD44780 framework configuration
 * @param state		Cursor state
 */
void hd44780fw_en_cursor(struct hd44780fw_conf* conf, uint8_t state);

/**
 * Sets blink/cursor position.
 *
 * @param conf 		HD44780 framework configuration
 * @param index		Blink/cursor position
 */
void hd44780fw_set_bc_index(struct hd44780fw_conf* conf, uint8_t index);

/**
 * Builds a custom character.
 *
 * @param conf 		HD44780 framework configuration
 * @param index		Custom character position (0 to 7)
 * @param rows		Custom character pixel rows
 */
void hd44780fw_build_cc(struct hd44780fw_conf* conf, uint8_t index,
	const uint8_t* rows);

/**
 * Concatenates a string at current v. cursor position.
 *
 * @param conf 		HD44780 framework configuration
 * @param msg		String to concatenate
 */
void hd44780fw_cat_string(struct hd44780fw_conf* conf, const char* msg);

/**
 * Concatenates a 16-bit unsigned integer at current v. cursor position.
 *
 * @param conf 		HD44780 framework configuration
 * @param u		16-bit unsigned integer to concatenate
 */
void hd44780fw_cat_uint16(struct hd44780fw_conf* conf, uint16_t u);

/**
 * Concatenates a 8-bit unsigned integer at current v. cursor position.
 *
 * @param conf 		HD44780 framework configuration
 * @param u		8-bit unsigned integer to concatenate
 */
void hd44780fw_cat_uint8(struct hd44780fw_conf* conf, uint8_t u);

/**
 * Concatenates a 16-bit signed integer at current v. cursor position.
 *
 * @param conf 		HD44780 framework configuration
 * @param i		16-bit signed integer to concatenate
 */
void hd44780fw_cat_int16(struct hd44780fw_conf* conf, int16_t i);

/**
 * Concatenates a 8-bit signed integer at current v. cursor position.
 *
 * @param conf 		HD44780 framework configuration
 * @param i		8-bit signed integer to concatenate
 */
void hd44780fw_cat_int8(struct hd44780fw_conf* conf, int8_t i);

/**
 * Concatenates a character at current v. cursor position.
 *
 * @param conf 		HD44780 framework configuration
 * @param ch		Character to concatenate
 */
void hd44780fw_cat_char(struct hd44780fw_conf* conf, char ch);

/**
 * Removes character from current v. cursor position.
 *
 * @param conf 		HD44780 framework configuration
 * @param rem		Number of characters to remove
 */
void hd44780fw_rem(struct hd44780fw_conf* conf, uint8_t rem);

#ifdef __cplusplus
}
#endif

#endif /* _HD44780FW_H */
