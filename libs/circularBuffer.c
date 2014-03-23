/**
 ******************************************************************************
 * @file	circularBuffer.c
 * @author	Hampus Sandberg
 * @version	0.1
 * @date	2013-02-10
 * @brief	Contains the function implementations for the circular buffer.
 *			Uses ATOMIC where necessary to avoid problem
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <util/atomic.h>
#include "assert.h"
#include "circularBuffer.h"

/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
/**
 * @brief	Initializes the circular buffer that is specified in the parameter
 * @param	CircularBuffer: the  buffer that should be initialized. If the 
 *			buffer already is initialized it will be reset
 * @retval	None
 */
void CIRCULAR_BUFFER_Init(volatile CircularBuffer_TypeDef* CircularBuffer)
{
	CircularBuffer->in = CircularBuffer->data;
	CircularBuffer->out = CircularBuffer->data;
	CircularBuffer->count = 0;
	CircularBuffer->initialized = 1;
}

/**
 * @brief	Insert an item in the front of the buffer
 * @param	CircularBuffer: the buffer to insert into
 * @param	Data: data to insert
 * @retval	None
 */
void CIRCULAR_BUFFER_Insert(volatile CircularBuffer_TypeDef* CircularBuffer, CIRCULARBUFFER_DATATYPE Data)
{
	*CircularBuffer->in = Data;
		
	if (++CircularBuffer->in == &CircularBuffer->data[CIRCULARBUFFER_SIZE])
		CircularBuffer->in = CircularBuffer->data;
	// Use atomic in case the count is bigger than 1 byte
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		CircularBuffer->count++;
	}
}

/**
 * @brief	Removes one item from the end of the buffer
 * @param	CircularBuffer: buffer to remove from
 * @retval	data removed from the end of the buffer
 */
CIRCULARBUFFER_DATATYPE CIRCULAR_BUFFER_Remove(volatile CircularBuffer_TypeDef* CircularBuffer)
{
	CIRCULARBUFFER_DATATYPE data = *CircularBuffer->out;
	    
    if (++CircularBuffer->out == &CircularBuffer->data[CIRCULARBUFFER_SIZE])
        CircularBuffer->out = CircularBuffer->data;
    // Use atomic in case the count is bigger than 1 byte
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        CircularBuffer->count--;
    }
    
    return data;
}

/**
 * @brief	Get the current count for the buffer
 * @param	CircularBuffer: the buffer to get the count for
 * @retval	the count value
 */
CIRCULARBUFFER_COUNTTYPE CIRCULAR_BUFFER_GetCount(volatile CircularBuffer_TypeDef* CircularBuffer)
{    
	CIRCULARBUFFER_COUNTTYPE count;
    // Use atomic in case the count is bigger than 1 byte
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        count = CircularBuffer->count;
    }

    return count;
}

/**
 * @brief	Check if the buffer is empty
 * @param	CircularBuffer: the buffer to check
 * @retval	(1) if it was empty, (0) otherwise
 */
uint8_t CIRCULAR_BUFFER_IsEmpty(volatile CircularBuffer_TypeDef* CircularBuffer)
{
	return (CIRCULAR_BUFFER_GetCount(CircularBuffer) == 0);
}

/**
 * @brief	Check if the buffer is full
 * @param	CircularBuffer: the buffer to check
 * @retval	(1) if it was full, (0) otherwise
 */
uint8_t CIRCULAR_BUFFER_IsFull(volatile CircularBuffer_TypeDef* CircularBuffer)
{
	return (CIRCULAR_BUFFER_GetCount(CircularBuffer) == CIRCULARBUFFER_SIZE);
}

/**
 * @brief	Flush the buffer by removing all the elements
 * @param	CircularBuffer: the buffer to flush
 * @retval	None
 */
void CIRCULAR_BUFFER_Flush(volatile CircularBuffer_TypeDef* CircularBuffer)
{
	CIRCULARBUFFER_COUNTTYPE bufferSize = CIRCULAR_BUFFER_GetCount(CircularBuffer);
	
	while (bufferSize != 0)
	{
		CIRCULAR_BUFFER_Remove(CircularBuffer);
		bufferSize--;
	}
}