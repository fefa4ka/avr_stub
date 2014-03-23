
#include <stdio.h>
#include <util/atomic.h>

#include "libs/uart.h"

#include "communication.h"




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

void process_command()
{
    char *pch;
    char cmdValue[16];

    switch (command_in[0]) {
        case 'S':
            if (command_in[1] == '?') {
                // Do the query action for S
                printf("sensitivity = %d\n", sensitivity);
            } else if (command_in[1] == '=') {
                sensitivity = parse_assignment(command_in);
            }
            break;
        case 'T':
            if (command_in[1] == '?') {
				get_temperature();

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