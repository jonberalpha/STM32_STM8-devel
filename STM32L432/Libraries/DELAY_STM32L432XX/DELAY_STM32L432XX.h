/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   DELAY_STM32L432XX.h                                           */
/* Autor:       Berger Jonas                                                  */
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
/*            U N T E R P R O G R A M M:    delay_init()                      */
/*                                                                            */
/* Aufgabe:   initialize SysTick-Timer                                        */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void delay_init(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    delay_us()                        */
/*                                                                            */
/* Aufgabe:   wait given us                                                   */
/* Input:     (uint32_t) - Zeit in us                                         */
/* return:                                                                    */
/******************************************************************************/
EXPORT void delay_us(uint32_t us);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    delay_ms()                        */
/*                                                                            */
/* Aufgabe:   wait given ms                                                   */
/* Input:     (uint32_t) - Zeit in ms                                         */
/* return:                                                                    */
/******************************************************************************/
EXPORT void delay_ms(uint32_t ms);

#undef EXPORT
#endif /* __DELAY_STM32L432XX_H */
/***************************  (C) PRIVATE  2020  *********END OF FILE**********/
