#include <stdio.h>

#include "libs/hd44780_low.h"
#include "libs/hd44780fw.h"

struct hd44780fw_conf g_fw_conf;
struct hd44780_l_conf g_low_conf;


int lcd_put(char ch, FILE* fh) {
	if (ch == '\n') {
		hd44780fw_clear(&g_fw_conf);
		
		return 0;
	}
	hd44780fw_cat_char(&g_fw_conf, ch);
	
	return 0;
}

// Настройка дисплея
void init_lcd(void) {
	DDRB = 0xff;
	DDRD = 0xff;
	g_low_conf.rs_i = 5;
	g_low_conf.rw_i = 4;
	g_low_conf.en_i = 3;
	g_low_conf.db7_i = 5;
	g_low_conf.db6_i = 6;
	g_low_conf.db5_i = 7;
	g_low_conf.db4_i = 0;
	g_low_conf.rs_port = g_low_conf.rw_port = g_low_conf.en_port = 
		g_low_conf.db4_port = &PORTB;
	g_low_conf.db7_port = g_low_conf.db6_port = g_low_conf.db5_port = &PORTD;
	
	g_low_conf.line1_base_addr = 0x00;
	g_low_conf.line2_base_addr = 0x40;
	g_low_conf.dl = HD44780_L_FS_DL_4BIT;
	g_fw_conf.low_conf = &g_low_conf;
	g_fw_conf.total_chars = 32;
	g_fw_conf.font = HD44780_L_FS_F_58;
	g_fw_conf.lines = HD44780_L_FS_N_DUAL;
	
	hd44780fw_init(&g_fw_conf);
	hd44780fw_clear(&g_fw_conf);
}

// Инициализация всего железа
void init_hardware(void) {
	init_lcd();
}



