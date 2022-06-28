/******************************************************************************/ 
/* File Name:   DELAY_STM32L432XX.h                                           */
/* Autor: 	    Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        25.09.2021                                                    */
/* Description: DELAY_STM32L432XX Library                                     */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef DELAY_STM32L432XX_H_INCLUDED
#define DELAY_STM32L432XX_H_INCLUDED

/*_______________________________ INCLUDES ___________________________________*/
#include <stm32l432xx.h>

#ifdef  DELAY_STM32L432XX_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/*___________________________ EXPORTED FUNCTIONS _____________________________*/
/******************************************************************************/
/*            SUBROUTINE:    delay_init()                                     */
/*                                                                            */
/* Purpose:   initialize SysTick-Timer                                        */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void delay_init(void);

/******************************************************************************/
/*            SUBROUTINE:    delay_us()                                       */
/*                                                                            */
/* Purpose:   wait given us                                                   */
/* Input:     (uint32_t) - Zeit in us                                         */
/* return:                                                                    */
/******************************************************************************/
EXPORT void delay_us(uint32_t us);

/******************************************************************************/
/*            SUBROUTINE:    delay_ms()                                       */
/*                                                                            */
/* Purpose:   wait given ms                                                   */
/* Input:     (uint32_t) - Zeit in ms                                         */
/* return:                                                                    */
/******************************************************************************/
EXPORT void delay_ms(uint32_t ms);

#undef EXPORT
#endif /* __DELAY_STM32L432XX_H */
