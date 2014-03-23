unsigned char	ow_nDevices;	
unsigned char	ds1820_nDevices;
unsigned char ds1820_Temperature[3][3];


int uart_put(char, FILE*);


// Инициализация всего железа
void init_hardware();

void init_uart();

// Получение температуры
void get_temperature();
