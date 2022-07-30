/******************************************************************************/
/* File Name:   TIMEOUT_STM32L432XX.h                                         */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        28.09.2021                                                    */
/* Description: TIMEOUT_STM32L432XX Library                                   */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef TIMEOUT_STM32L432XX_H_INCLUDED
#define TIMEOUT_STM32L432XX_H_INCLUDED

/*_______________________________ INCLUDES ___________________________________*/
#include <stm32l432xx.h>

#ifdef  TIMEOUT_STM32L432XX_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/*_______________________________ DEFINES ___________________________________*/
#define US 1 // timeout in us
#define MS 2 // timeout in ms

/*___________________________ EXPORTED FUNCTIONS _____________________________*/
/******************************************************************************/
/*            SUBROUTINE:    timeout_init()                                   */
/*                                                                            */
/* Purpose:   initialize Timer1 (blocking) & Timer2 (nonblocking)             */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void timeout_init(void);

/******************************************************************************/
/*            SUBROUTINE:    _tim_timeout_blocking()                          */
/*                                                                            */
/* Purpose:   wait given time blocking                                        */
/* Input:     (uint32_t) - time, (int) - MS or US                             */
/* return:                                                                    */
/******************************************************************************/
EXPORT void _tim_timeout_blocking(uint32_t time, int unit);

/******************************************************************************/
/*            SUBROUTINE:    _tim_timeout_nonblocking_with_callback()         */
/*                                                                            */
/* Purpose:   execute function non blocking after given time                  */
/* Input:     (uint32_t) - time, (uint16_t) - timeunit, adress of pointer2func*/
/* return:                                                                    */
/******************************************************************************/
EXPORT void _tim_timeout_nonblocking_with_callback(uint32_t time, uint16_t unit, void (*func)());

#undef EXPORT
#endif /* __TIMEOUT_STM32L432XX_H */
