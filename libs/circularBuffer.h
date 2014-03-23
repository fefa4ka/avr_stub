/**
 ******************************************************************************
 * @file	circularBuffer.h
 * @author	Hampus Sandberg
 * @version	0.1
 * @date	2013-02-10
 * @brief	A circular buffer with functionality to insert and remove items
 *			The buffer will use CIRCULARBUFFER_SIZE + 5 bytes (37 bytes default) 
 *			of SRAM
 ******************************************************************************
 */

#include <stdio.h>


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

/* Includes ------------------------------------------------------------------*/
/* 
 * Put a file named "board.h" in the project root directory and add ".." as a directory
 * Here we define CIRCULARBUFFER_SIZE, CIRCULARBUFFER_DATATYPE
 */
// #include <board.h>

/* Defines -------------------------------------------------------------------*/
#ifndef CIRCULARBUFFER_SIZE
#warning "CIRCULARBUFFER_SIZE is not defined. Will now define it to 64"
#define CIRCULARBUFFER_SIZE	64
#endif

#define BUFFER_NEARLY_FULL	CIRCULARBUFFER_SIZE - 16

#ifndef CIRCULARBUFFER_DATATYPE
#warning "CIRCULARBUFFER_DATATYPE is not defined. Will now define it to volatile uint8_t"
#define CIRCULARBUFFER_DATATYPE	volatile uint8_t
#endif

#if (CIRCULARBUFFER_SIZE <= 0xFF)
#define CIRCULARBUFFER_COUNTTYPE    volatile uint8_t
#else
#define CIRCULARBUFFER_COUNTTYPE    volatile uint16_t
#endif
/* Typedefs ------------------------------------------------------------------*/
/**
 * @brief  Struct to handle a circular buffer
 */
typedef struct
{
	CIRCULARBUFFER_DATATYPE *in;							/** Pointer that points at the head where 
																data should be written */
	CIRCULARBUFFER_DATATYPE *out;							/** Pointer that points at the tail where 
																data should be read */
	CIRCULARBUFFER_DATATYPE data[CIRCULARBUFFER_SIZE];		/** The actual buffer */
	CIRCULARBUFFER_COUNTTYPE count;							/** A counter for how much data there is
																in the buffer */
	volatile uint8_t initialized;									/** Variable that is set once init has been
																done on the buffer */
} CircularBuffer_TypeDef;

/* Function prototypes -------------------------------------------------------*/
void CIRCULAR_BUFFER_Init(volatile CircularBuffer_TypeDef* CircularBuffer);

void CIRCULAR_BUFFER_Insert(volatile CircularBuffer_TypeDef* CircularBuffer, CIRCULARBUFFER_DATATYPE Data);
CIRCULARBUFFER_DATATYPE CIRCULAR_BUFFER_Remove(volatile CircularBuffer_TypeDef* CircularBuffer);

CIRCULARBUFFER_COUNTTYPE CIRCULAR_BUFFER_GetCount(volatile CircularBuffer_TypeDef* CircularBuffer);
uint8_t CIRCULAR_BUFFER_IsEmpty(volatile CircularBuffer_TypeDef* CircularBuffer);
uint8_t CIRCULAR_BUFFER_IsFull(volatile CircularBuffer_TypeDef* CircularBuffer);
void CIRCULAR_BUFFER_Flush(volatile CircularBuffer_TypeDef* CircularBuffer);

#endif /* CIRCULARBUFFER_H_ */