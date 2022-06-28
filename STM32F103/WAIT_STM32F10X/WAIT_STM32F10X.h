/******************************************************************************/
/* File Name:   WAIT_STM32F10X.h                                              */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        07.05.2020                                                    */
/* Description: Wait for x us or x ms                                         */
/******************************************************************************/
/* History: 	V1.00  creation                                                 */
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
/*            SUBROUTINE:    wait_init()                                      */
/*                                                                            */
/* Purpose:   init Wait                                                       */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void wait_init(void);

/******************************************************************************/
/*            SUBROUTINE:    wait_us()                                        */
/*                                                                            */
/* Purpose:   wartet x us ab                                                  */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void wait_us(uint32_t us);

/******************************************************************************/
/*            SUBROUTINE:    wait_ms()                                        */
/*                                                                            */
/* Purpose:   wartet x ms ab                                                  */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void wait_ms(uint32_t ms);


#undef EXPORT
#endif /* __WAIT_STM32F10X_H */
