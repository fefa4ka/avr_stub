/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>

#include "init.h"

int main(void) {
	init_hardware();
	
	stdout = fdevopen(lcd_put, NULL);

	printf("test");
	for (;;);
	
	fclose(stdout);
	
	return 0;
}



