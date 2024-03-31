/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   SYS_CLK_STM32L432XX.h                                         */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        28.09.2021                                                    */
/* Description: SYS_CLK_STM32L432XX Library                                   */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef SYS_CLK_STM32L432XX_H_INCLUDED
#define SYS_CLK_STM32L432XX_H_INCLUDED

/*_______________________________ INCLUDES ___________________________________*/
#include <stm32l432xx.h>

#ifdef  SYS_CLK_STM32L432XX_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/*___________________________ EXPORTED FUNCTIONS _____________________________*/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    set_clock_80MHz()                 */
/*                                                                            */
/* Aufgabe:   set SystemCoreClock to 80 MHz                                   */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void set_clock_80MHz(void);

#undef EXPORT
#endif /* __SYS_CLK_STM32L432XX_H */
/***************************  (C) PRIVATE  2020  *********END OF FILE**********/
