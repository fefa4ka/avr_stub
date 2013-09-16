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

#include <avr/io.h>
#include <util/delay.h>

#include "customprocs.h"
#include "hd44780_low.h"
#include "hd44780fw.h"

void hd44780fw_init(struct hd44780fw_conf* conf) {
	/* Structure initialization */
	conf->half_chars = conf->total_chars / 2;
	conf->blink_en = HD44780FW_DEF_BLINK_ST;
	conf->cur_en = HD44780FW_DEF_CUR_ST;
	conf->last_index = 0;
	conf->last_bc_index = 0;
	
	/* Device initialization: */
	hd44780_l_init(conf->low_conf, conf->lines, conf->font,
		HD44780_L_EMS_ID_INC, HD44780_L_EMS_S_OFF);
		
	/* Turn on display and set blink/cursor according to default values: */
	hd44780_l_disp(conf->low_conf, HD44780_L_DISP_D_ON,
		HD44780FW_DEF_CUR_ST, HD44780FW_DEF_BLINK_ST);
}

void hd44780fw_fini(struct hd44780fw_conf* conf) {
}

void hd44780fw_put(struct hd44780fw_conf* conf, char ch) {
	hd44780fw_cat_char(conf, ch);
}

void hd44780fw_write(struct hd44780fw_conf* conf, const char* msg,
uint8_t index, uint8_t cb) {
	uint8_t msg_len = cp_strlen(msg),
		up_len = 0,
		i;

	/* Disable blink while appending chars: */
	const uint8_t blink_en_bkup = conf->blink_en;
	const uint8_t cur_en_bkup = conf->cur_en;
	hd44780fw_en_blink(conf, HD44780_L_DISP_B_OFF);
	hd44780fw_en_cursor(conf, HD44780_L_DISP_C_OFF);
	
	/* Frequently used constants: */
	const uint8_t line1_base_addr = conf->low_conf->line1_base_addr;
	const uint8_t line2_base_addr = conf->low_conf->line2_base_addr;
	const uint8_t total_chars = conf->total_chars;
	const uint8_t half_chars = conf->half_chars;	
	
	if (cb) {
		hd44780fw_clear(conf);
	}
	if (index >= total_chars || total_chars - index < msg_len) {
		return;
	}
	if (index < half_chars) {
		if (msg_len + index < line1_base_addr + half_chars) {
			up_len = msg_len;
		} else {
			up_len = half_chars - index;
		}
		hd44780_l_set_ddram_addr(conf->low_conf, index + line1_base_addr);
		for (i = 0; i < up_len; ++i) {
			hd44780_l_write(conf->low_conf, msg[i]);
		}
		hd44780_l_set_ddram_addr(conf->low_conf, line2_base_addr);
		for (i = up_len; i < msg_len; ++i) {
			hd44780_l_write(conf->low_conf, msg[i]);
		}
	} else {
		hd44780_l_set_ddram_addr(conf->low_conf,
			index + line2_base_addr - half_chars);
		for (i = 0; i < msg_len; ++i) {
			hd44780_l_write(conf->low_conf, msg[i]);
		}
	}

	/* Enable last blink/cursor state: */
	hd44780fw_set_bc_index(conf, conf->last_bc_index);
	hd44780fw_en_blink(conf, blink_en_bkup);
	hd44780fw_en_cursor(conf, cur_en_bkup);
	
	/* Fix v. cursor: */
	conf->last_index = index + msg_len;
}

void hd44780fw_clear(struct hd44780fw_conf* conf) {
	hd44780_l_clear_disp(conf->low_conf); /* Device clear function */
	conf->last_index = 0; /* Reinitialize v. cursor */
	hd44780fw_set_bc_index(conf, 0);
}

void hd44780fw_en_blink(struct hd44780fw_conf* conf, uint8_t state) {
	hd44780_l_disp(conf->low_conf, HD44780_L_DISP_D_ON,
		conf->cur_en ? HD44780_L_DISP_C_ON : HD44780_L_DISP_C_OFF,
		state ? HD44780_L_DISP_B_ON : HD44780_L_DISP_B_OFF);
	conf->blink_en = state;
}

void hd44780fw_en_cursor(struct hd44780fw_conf* conf, uint8_t state) {
	hd44780_l_disp(conf->low_conf, HD44780_L_DISP_D_ON,
		state ? HD44780_L_DISP_C_ON : HD44780_L_DISP_C_OFF,
		conf->blink_en ? HD44780_L_DISP_B_ON : HD44780_L_DISP_B_OFF);
	conf->cur_en = state;
}

void hd44780fw_set_bc_index(struct hd44780fw_conf* conf, uint8_t index) {
	const uint8_t line1_base_addr = conf->low_conf->line1_base_addr;
	const uint8_t line2_base_addr = conf->low_conf->line2_base_addr;
	
	if (index >= conf->total_chars) {
		return;
	}
	conf->last_bc_index = index;
	
	if (index < conf->half_chars) {
		hd44780_l_set_ddram_addr(conf->low_conf,
			index + line1_base_addr);
	} else {
		hd44780_l_set_ddram_addr(conf->low_conf,
			index + line2_base_addr - conf->half_chars);
	}
}

void hd44780fw_build_cc(struct hd44780fw_conf* conf, uint8_t index,
const uint8_t* rows) {
	uint8_t i;
	uint8_t mult = (conf->font == HD44780_L_FS_F_58 ? 8 : 10);
	if (conf->font == HD44780_L_FS_F_510) {
		return;		/* Not yet implemented */
	}
	if (index > 7) {
		return;
	}
	hd44780_l_set_cgram_addr(conf->low_conf, index * mult);
	for (i = 0; i < mult; ++i) {
		hd44780_l_write(conf->low_conf, rows[i]); /* Write cur. row */
	}
}

void hd44780fw_cat_string(struct hd44780fw_conf* conf, const char* msg) {
	hd44780fw_write(conf, msg, conf->last_index, 0);
}

void hd44780fw_cat_uint16(struct hd44780fw_conf* conf, uint16_t u) {
	hd44780fw_cat_string(conf, cp_utoa(conf->buf, HD44780FW_BUF_SIZE,
		u, HD44780FW_DEF_NB_BASE));
}

void hd44780fw_cat_uint8(struct hd44780fw_conf* conf, uint8_t u) {
	hd44780fw_cat_uint16(conf, (uint16_t) u);
}

void hd44780fw_cat_int16(struct hd44780fw_conf* conf, int16_t i) {
	hd44780fw_cat_string(conf, cp_itoa(conf->buf, HD44780FW_BUF_SIZE,
		i, HD44780FW_DEF_NB_BASE));
}

void hd44780fw_cat_int8(struct hd44780fw_conf* conf, int8_t i) {
	hd44780fw_cat_int16(conf, (int16_t) i);
}

void hd44780fw_cat_char(struct hd44780fw_conf* conf, char ch) {
	conf->buf[0] = ch;
	conf->buf[1] = '\0';
	hd44780fw_cat_string(conf, conf->buf);
}

void hd44780fw_rem(struct hd44780fw_conf* conf, uint8_t rem) {
	uint8_t i;
	
	if (rem > conf->last_index) {
		rem = conf->last_index;
	}
	conf->last_index -= rem;
	for (i = 0; i < rem; ++i) {
		hd44780fw_cat_char(conf, HD44780FW_DEF_REM_CHAR);
	}
	conf->last_index -= rem;
}
