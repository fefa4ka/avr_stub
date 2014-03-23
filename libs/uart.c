/**
 ******************************************************************************
 * @file	uart.c
 * @author	Hampus Sandberg
 * @version	0.1
 * @date	2013-02-12
 * @brief	Contains functions to manage the SPI-peripheral on ATmega328x
 *			- Initialization
 *			- Write data
 *			- Buffer functionality if interrupt is used
 *			- Receive data
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "assert.h"
#include "circularBuffer.h"
#include "uart.h"

/* Private defines -----------------------------------------------------------*/
#define UART_DDR		DDRD
#define UART_PORT		PORTD
#define UART_RX_PIN		PORTD0
#define UART_TX_PIN		PORTD1

/* Private variables ---------------------------------------------------------*/
volatile UART_Status_TypeDef _uartRXstatus;
volatile CircularBuffer_TypeDef _uartBufferRX;
volatile UART_Status_TypeDef _uartTXstatus;
volatile CircularBuffer_TypeDef _uartBufferTX;
uint8_t _uartInitStatus;

/* Strings located in FLASH memory */
const char McuType[] PROGMEM = "\rATmega126x\r";

/* Private functions ---------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
/**
 * @brief	Initializes the UART peripheral according to the specified parameters in the UART_InitStruct.
 * @param	UART_InitStruct: pointer to a UART_Init_TypeDef structure that contains
 *			the configuration information for the UART peripheral.
 * @retval	None
 */
void UART_Init(UART_Init_TypeDef *UART_InitStruct)
{
	// Check parameters
	assert_param(IS_UART_BAUD_RATE(UART_InitStruct->UART_BaudRate));
	
	// Set pins
	UART_DDR |= (1 << UART_TX_PIN);
	UART_DDR &= ~(1 << UART_RX_PIN);
	
	_uartRXstatus = UART_RX_BUFFER_EMPTY;
	_uartTXstatus = UART_TX_BUFFER_EMPTY;
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	if (UART_InitStruct->UART_BaudRate == UART_BAUD_57600 || UART_InitStruct->UART_BaudRate == UART_BAUD_115200)
	{	
		UCSR0A |= (1 << U2X0);
	}		
	UBRR0 = UART_InitStruct->UART_BaudRate;
	
	// Initialize the buffers
	CIRCULAR_BUFFER_Init(&_uartBufferRX);
	CIRCULAR_BUFFER_Init(&_uartBufferTX);
	
	sei();
	_uartInitStatus = 1;
}

/**
 * @brief	Writes data to the TX buffer and enables the "Data Register Empty" interrupt so that 
 *			the data can be written to the UART
 * @param	Data: data to be written to the UART
 * @retval	None
 */
void UART_Write(const uint8_t Data)
{
	// TODO: Add timemout
	while (CIRCULAR_BUFFER_IsFull(&_uartBufferTX));
	CIRCULAR_BUFFER_Insert(&_uartBufferTX, Data);
	// Activate the "Data Register Empty" interrupt
	UCSR0B |= (1 << UDRIE0);
}

/**
 * @brief	Write a string to the USART
 * @param	String: The string to write
 * @retval	None
 */
void UART_WriteString(const char *String)
{
	while (*String != 0x00)
	{
		UART_Write(*String++);
	}
}

/**
 * @brief	Write a string to the USART from FLASH memory
 * @param	String: The string to write located in FLASH
 * @retval	None
 */
void UART_WriteString_P(const char *String)
{
	while (pgm_read_byte(String) != 0x00)
	{
		UART_Write(pgm_read_byte(String++));
	}
}

void UART_WriteUintAsString(uint8_t Number)
{
	char str[4];
	utoa(Number, str, 10);
	UART_WriteString(str);
}

void UART_WriteUint16AsString(uint16_t Number)
{
	char str[8];
	utoa(Number, str, 10);
	UART_WriteString(str);
}

void UART_WriteInt16AsString(int16_t Number)
{
	char str[10];
	itoa(Number, str, 10);
	UART_WriteString(str);
}

void UART_WriteHexByte(uint8_t theByte, uint8_t prefix)
{
	char str[3];
	utoa(theByte, str, 16);
	if (prefix) UART_WriteString("0x");
	UART_WriteString(str);
}

/**
 * @brief	Reads the next value in the RX buffer
 * @param	None
 * @retval	The next value in the buffer that should be removed or [0] if the buffer is empty
 * @none	Before using UART_Read() a call to UART_DataAvailable() should be made to ensure 
 *			there is data to read
 */
uint8_t UART_Read()
{
	if (!CIRCULAR_BUFFER_IsEmpty(&_uartBufferRX))
		return CIRCULAR_BUFFER_Remove(&_uartBufferRX);
	else
	{
		_uartRXstatus = UART_RX_BUFFER_EMPTY;
		return 0;
	}	
}

/**
 * @brief	Gets the current size of the RX buffer
 * @param	None
 * @retval	The current size of the RX buffer
 */
uint8_t UART_DataAvailable()
{
	return CIRCULAR_BUFFER_GetCount(&_uartBufferRX);
}

/**
 * @brief	Gets the current status of the UART
 * @param	None
 * @retval	The current status of the UART
 */
UART_StatusInfo_TypeDef UART_GetStatus()
{
	UART_StatusInfo_TypeDef currentStatus;
	currentStatus.rxStatus = _uartRXstatus;
	currentStatus.txStatus = _uartTXstatus;
	
	return currentStatus;
}

/**
 * @brief	Waits in a while loop until all bytes in the buffer has been transmitted
 * @param	None
 * @retval	None
 */
void UART_WaitForTxComplete()
{
	while (_uartTXstatus == UART_TX_TRANSMITTING);
}

/**
 * @brief	Checks to see if UART has been initialized
 * @param	None
 * @retval	None
 */
uint8_t UART_Initialized()
{
	return _uartInitStatus;
}

/* Interrupt Service Routines ------------------------------------------------*/
/**
 * @brief	Executes when there is unread data present in the receive buffer
 */
ISR(USART_RX_vect)
{
	uint8_t data = UDR0;
	if (CIRCULAR_BUFFER_IsFull(&_uartBufferRX))
		_uartRXstatus = UART_RX_BUFFER_FULL;
	else
	{
		CIRCULAR_BUFFER_Insert(&_uartBufferRX, data);
		_uartRXstatus = UART_DATA_IN_RX_BUFFER;
	}
}

/**
 * @brief	Executes when the data register (UDR0) is empty
 */
ISR(USART_UDRE_vect)
{
	if (CIRCULAR_BUFFER_IsEmpty(&_uartBufferTX))
	{
		// No data to transmit so disable "Data Register Empty" interrupt
		UCSR0B &= ~(1 << UDRIE0);
		_uartTXstatus = UART_TX_BUFFER_EMPTY;
	}
	else
	{
		// Data is available so remove it from the buffer and transmit
		_uartTXstatus = UART_TX_TRANSMITTING;
		UDR0 = CIRCULAR_BUFFER_Remove(&_uartBufferTX);
	}
}