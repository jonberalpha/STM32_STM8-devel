/******************************************************************************/
/*   (C) Copyright PRIVATE  2020 All rights reserved. AUSTRIA                 */
/*                                                                            */
/* File Name:   WAIT_STM32F10X.h                                              */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        07.05.2020                                                    */
/* Description: Warte x us bzw x ms ab                                        */
/*              Header-File stellt defines und Prototypen zur Verfuegung      */
/*              Befehle sind hier ausprogrammiert                             */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef WAIT_STM32F10X_H_INCLUDED
#define WAIT_STM32F10X_H_INCLUDED

/*________________________________ INCLUDES __________________________________*/
#include <stm32f10x.h>

#ifdef  WAIT_STM32F10X_MOD
#define EXPORT
#else
#define EXPORT extern
#endif 

/*____________________________ EXPORTED FUNCTIONS ____________________________*/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    wait_init()                       */
/*                                                                            */
/* Aufgabe:   init Wait                                                       */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void wait_init(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    wait_us()                         */
/*                                                                            */
/* Aufgabe:   wartet x us ab                                                  */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void wait_us(uint32_t us);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    wait_ms()                         */
/*                                                                            */
/* Aufgabe:   wartet x ms ab                                                  */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void wait_ms(uint32_t ms);


#undef EXPORT
#endif /* __WAIT_STM32F10X_H */
/***************************  (C) PRIVATE  2020  *********END OF FILE**********/
