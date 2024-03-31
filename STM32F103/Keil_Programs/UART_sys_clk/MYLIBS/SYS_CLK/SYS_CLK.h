/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   SYS_CLK.h                                                     */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        25.08.2020                                                    */
/* Description: SYS_CLK Library                                               */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef SYS_CLK_H_INCLUDED
#define SYS_CLK_H_INCLUDED

/*_______________________________ INCLUDES ___________________________________*/
#include <stm32f10x.h>

#ifdef  SYS_CLK_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/*___________________________ EXPORTED FUNCTIONS _____________________________*/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    set_clock_72MHz()                 */
/*                                                                            */
/* Aufgabe:   set SystemCoreClock to 72 MHz                                   */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void set_clock_72MHz(void);

#undef EXPORT
#endif /* __SYS_CLK_H */
/***************************  (C) PRIVATE  2020  *********END OF FILE**********/
