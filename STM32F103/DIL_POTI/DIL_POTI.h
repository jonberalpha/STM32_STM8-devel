/******************************************************************************/
/* File Name:   DIL_POTI.h                                                    */
/* Autor: 	    Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        07.05.2020                                                    */
/* Description: ADC Functions                                                 */
/******************************************************************************/
/* History: 	V1.00  creation                                                 */
/******************************************************************************/
#ifndef DIL_POTI_H_INCLUDED
#define DIL_POTI_H_INCLUDED

/*________________________________ INCLUDES __________________________________*/
#include <stm32f10x.h>

#ifdef  DIL_POTI_MOD
#define EXPORT
#else
#define EXPORT extern
#endif 

/*____________________________ EXPORTED FUNCTIONS ____________________________*/
/******************************************************************************/
/*               SUBROUTINE:    poti_convert(void)                            */
/*                                                                            */
/* Purpose: delivers value of Portline PC4 (ADC1 Kanal 14)                    */
/* Input:   none                                                              */
/* return:  converted value (12Bit right aligned)                             */
/******************************************************************************/
EXPORT unsigned short int poti_convert(void);

#undef EXPORT
#endif /* __DIL_POTI_H */
