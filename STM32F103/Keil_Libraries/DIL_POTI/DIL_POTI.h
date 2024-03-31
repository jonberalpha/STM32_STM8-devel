/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   DIL_POTI.h                                                    */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        07.05.2020                                                    */
/* Description: ADC Functions                                                 */
/*              Header-File stellt defines und Prototypen zur Verfuegung      */
/*              Befehle sind hier ausprogrammiert                             */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
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
/*               U N T E R P R O G R A M M:    poti_convert(void)             */
/*                                                                            */
/* Aufgabe: delivers value of Portline PC4 (ADC1 Kanal 14)                    */
/* Input:   none                                                              */
/* return:  converted value (12Bit right aligned)                             */
/******************************************************************************/
EXPORT unsigned short int poti_convert(void);

#undef EXPORT
#endif /* __DIL_POTI_H */
/***************************  (C) PRIVATE  2020  *********END OF FILE**********/
