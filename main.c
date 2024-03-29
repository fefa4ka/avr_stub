/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "libs/uart.h"

#include "init.h"

#include "libs/ds1307.h"
#include "libs/dht11.h"

volatile unsigned char data_in[8];
unsigned char command_in[8];

volatile unsigned char data_count;
volatile unsigned char command_ready;

int main(void) {
	init_hardware();

	stdout = fdevopen(uart_put, NULL);


	for (;;) {

        DDRC |= (1<<PC2); //output
        PORTC |= (1<<PC2); //high
        _delay_ms(100);
        PORTC &= ~(1<<PC2); 
        _delay_ms(50);
		if( UART_DataAvailable() ) {
			uart_get_command();
		}

		if(command_ready == 1) {
			process_command();

			command_ready = 0;
		}

	}
	
	fclose(stdout);
	
	return 0;
}




void uart_get_command() {
	while( UART_DataAvailable() ) {
		data_in[data_count] = UART_Read();

	    // End of line!
	    if (data_in[data_count] == '\n') {
	        command_ready = 1;
	        copy_command();

	        // Reset to 0, ready to go again
	        data_count = 0;
	    } else {
	        data_count++;
	    }
	}
}

void copy_command() 
{
    // The USART might interrupt this - don't let that happen!
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        // Copy the contents of data_in into command_in
        memcpy(command_in, data_in, 8);

        // Now clear data_in, the USART can reuse it now
        memset(data_in[0], 0, 8);
    }
}

unsigned long parse_assignment (char input[16])
{
    char *pch;
    char cmdValue[16];
    // Find the position the equals sign is
    // in the string, keep a pointer to it
    pch = strchr(command_in, '=');
    // Copy everything after that point into
    // the buffer variable
    strcpy(cmdValue, pch+1);
    // Now turn this value into an integer and
	// return it to the caller.
    return atoi(cmdValue);
}

unsigned int sensitivity;
DS1307Date datetime;

void process_command()
{
    char *pch;
    char cmdValue[16];

    switch (command_in[0]) {
        case 'D':
            if (command_in[1] == '?') {
                ds1307_read_date(&datetime);
   
                printf("%d.%d.%d %d:%d:%d\n", datetime.day, datetime.month, datetime.year, datetime.hour, datetime.minute, datetime.second);

            } else if (command_in[1] == '=') {
                sensitivity = parse_assignment(command_in);
            }
            break;
        case 'H':
            if (command_in[1] == '?') {   
                printf("Humidity: %d\nTemperature: %d\n", dht11_gethumidity(), dht11_gettemperature());

            } else if (command_in[1] == '=') {
                sensitivity = parse_assignment(command_in);
            }
            break;
        case 'T':
            if (command_in[1] == '?') {
				get_temperature();
                printf("Temperature: ");
				for(unsigned char i = 0; i < ds1820_nDevices; i++) {
		            printf("%d.%d C\n", ds1820_Temperature[i][0], ds1820_Temperature[i][1]);
				}

            } else if (command_in[1] == '=') {
            	printf("Set minimum temperature");
                // Do the set action for M
            }
            break;
        default:
            break;
    }
}