#include <stdio.h>

#include <util/delay.h>

#include "libs/onewire.h"
#include "libs/ds18x20.h"
#include "libs/uart.h"
#include "libs/ds1307.h"



int uart_put(char ch, FILE* fh) {
	
	UART_Write(ch);
	
	return 0;
}


unsigned char	ow_nDevices;	
unsigned char	ow_DevicesIDs[MAXDEVICES][8];
unsigned char	ds1820_nDevices;
unsigned char	ds1820_Devices[MAXDEVICES];
unsigned char	ds1820_Temperature[MAXDEVICES][3];

unsigned char ow_search_devices() {
	unsigned char	i;
   	unsigned char	id[OW_ROMCODE_SIZE];
   	unsigned char	diff, sensors_count;

	sensors_count = 0;

	for( diff = OW_SEARCH_FIRST; diff != OW_LAST_DEVICE && sensors_count < MAXDEVICES ; )
    { 

		OW_FindROM( &diff, &id[0] );

      	if( diff == OW_PRESENCE_ERR ) break;

      	if( diff == OW_DATA_ERR )	break;

      	for (i = 0; i < OW_ROMCODE_SIZE; i++)
         	ow_DevicesIDs[sensors_count][i] = id[i];
		
		sensors_count++;
    }

	return sensors_count;
}

void init_thermometers() {
	unsigned char sensors_count = 0;

	// Ищем устройства на линии
	ow_nDevices = ow_search_devices();

	for (unsigned char i = 0; i < ow_nDevices; i++) 
	{

		switch (ow_DevicesIDs[i][0])
		{
			case OW_DS18B20_FAMILY_CODE: { 
				ds1820_Devices[sensors_count] = i;

				sensors_count++;
			} break;
		}
	}

	ds1820_nDevices = sensors_count;
	get_temperature();

}

void get_temperature() {
	unsigned char device_id;

	for (unsigned char i = 0; i < ds1820_nDevices; i++) 
	{
		device_id = ds1820_Devices[i];
		switch (ow_DevicesIDs[device_id][0])
		{
			case OW_DS18B20_FAMILY_CODE: { 
				DS18x20_StartMeasure(ow_DevicesIDs[device_id]); 
				_delay_ms(800); 
				unsigned char	data[2]; 
				DS18x20_ReadData(ow_DevicesIDs[device_id], data); 
				
				DS18x20_ConvertToThemperature(data, ds1820_Temperature[i]); // преобразовываем температуру в человекопонятный вид
			} break;
		}
	}
}

void init_uart() {
	UART_Init_TypeDef uart_conf = { UART_BAUD_38400 };

	UART_Init(&uart_conf);

}

// Инициализация всего железа
void init_hardware(void) {
	init_uart();
	
	init_thermometers();

	ds1307_init();

}



