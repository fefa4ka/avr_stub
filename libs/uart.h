/**
 ******************************************************************************
 * @file	uart.h
 * @author	Hampus Sandberg
 * @version	0.1
 * @date	2013-02-12
 * @brief	Contains function prototypes, constants to manage the UART-peripheral
 *			on ATmega328x
 ******************************************************************************
 */
#include <stdio.h>

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UART_H_
#define UART_H_

/* Includes ------------------------------------------------------------------*/
/* Defines -------------------------------------------------------------------*/
/* Typedefs ------------------------------------------------------------------*/
/**
 * @brief  UART baud rate
 */
typedef enum  
{
	UART_BAUD_9600 =	51,
	UART_BAUD_19200 =	25,
	UART_BAUD_38400 =	12,
	UART_BAUD_57600 =	16,
	UART_BAUD_115200 =	8
} UART_Baud_Rate_TypeDef;
#define IS_UART_BAUD_RATE(BAUD) (((BAUD) == UART_BAUD_9600) || ((BAUD) == UART_BAUD_19200) || \
								((BAUD) == UART_BAUD_38400) || ((BAUD) == UART_BAUD_57600) || \
								((BAUD) == UART_BAUD_115200))
								
/**
 * @brief  UART status
 */
typedef enum
{
	UART_DATA_IN_RX_BUFFER =	0x00,
	UART_RX_BUFFER_EMPTY =		0x01,
	UART_RX_BUFFER_FULL =		0x02,
	UART_TX_BUFFER_EMPTY =		0x03,
	UART_TX_TRANSMITTING =		0x04
	
} UART_Status_TypeDef;

/**
 * @brief  Struct to hold RX & TX status for the UART
 */
typedef struct  
{
	UART_Status_TypeDef rxStatus;
	UART_Status_TypeDef txStatus;
} UART_StatusInfo_TypeDef;

/**
 * @brief  SPI Init structure definition
 */
typedef struct
{
    UART_Baud_Rate_TypeDef UART_BaudRate;		/** Specifies the baud rate for the UART peripheral.
													This parameter can be any value of UART_Baud_Rate_TypeDef */
} UART_Init_TypeDef;

/* Function prototypes -------------------------------------------------------*/
// void UART_Init(const uint8_t theBaudRate, void(*theUartManageFunc)(uint8_t));
void UART_Init(UART_Init_TypeDef *UART_InitStruct);
void UART_Write(const uint8_t Data);
void UART_WriteString(const char *String);
void UART_WriteString_P(const char *String);
void UART_WriteUintAsString(uint8_t Number);
void UART_WriteUint16AsString(uint16_t Number);
void UART_WriteInt16AsString(int16_t Number);
void UART_WriteHexByte(uint8_t theByte, uint8_t prefix);

uint8_t UART_Read();
uint8_t UART_DataAvailable();
UART_StatusInfo_TypeDef UART_GetStatus();
void UART_WaitForTxComplete();
uint8_t UART_Initialized();

#endif /* UART_H_ */