/**
 ******************************************************************************
 * @file    assert.h
 * @author  Hampus Sandberg
 * @version 0.1
 * @date    2013-02-10
 * @brief   Contains macros and functions to handle parameter checks
 ******************************************************************************
 */

#include <stdio.h>


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ASSERT_H_
#define ASSERT_H_

/* Includes ------------------------------------------------------------------*/
/* Typedefs ------------------------------------------------------------------*/
/* Defines -------------------------------------------------------------------*/

/* Macros --------------------------------------------------------------------*/
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))

/* Function prototypes -------------------------------------------------------*/
void assert_failed(uint8_t* file, uint32_t line);

#endif
