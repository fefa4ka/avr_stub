volatile unsigned char data_in[8];
unsigned char command_in[8];

volatile unsigned char data_count;
volatile unsigned char command_ready;

void uart_get_command();
void copy_command();
long parse_assignment(char);
void process_command();

