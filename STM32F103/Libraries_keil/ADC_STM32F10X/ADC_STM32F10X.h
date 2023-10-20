/******************************************************************************/
/* File Name:   ADC_STM32F10X.h                                               */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        23.07.2020                                                    */
/* Description: Exported Functions                                            */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef ADC_STM32F10X_H_INCLUDED
#define ADC_STM32F10X_H_INCLUDED

/*________________________________ INCLUDES __________________________________*/
#include <stm32f10x.h>

#ifdef  ADC_STM32F10X_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/*____________________________ EXPORTED FUNCTIONS ____________________________*/
/******************************************************************************/
/*               SUBROUTINE:    adc_convert()                                 */
/*                                                                            */
/* Aufgabe: delivers value of ADC1 Kanal x                                    */
/* Input:   int channel                                                       */
/* return:  converted value (12Bit right aligned)                             */
/******************************************************************************/
EXPORT unsigned short int adc_convert(int channel);

#undef EXPORT
#endif /* __ADC_STM32F10X_H */
