/******************************************************************************/
/* File Name:   ADC_POTI_STM32L432XX.h                                        */
/* Autor: 	    Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        28.09.2021                                                    */
/* Description: ADC_POTI_STM32L432XX Library                                  */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef ADC_POTI_STM32L432XX_H_INCLUDED
#define ADC_POTI_STM32L432XX_H_INCLUDED

/*_______________________________ INCLUDES ___________________________________*/
#include <stm32l432xx.h>

#ifdef  ADC_POTI_STM32L432XX_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/*___________________________ EXPORTED FUNCTIONS _____________________________*/
/******************************************************************************/
/*            SUBROUTINE:    adc_read_poti()                                  */
/*                                                                            */
/* Purpose:   read Poti-Value on PA7 (ADC1/channel 12)                        */
/* Input:                                                                     */
/* return:    (unsigned int) - poti value                                     */
/******************************************************************************/
EXPORT unsigned int adc_read_poti(void);

#undef EXPORT
#endif /* __ADC_POTI_STM32L432XX_H */
/***************************  (C) PRIVATE  2020  *********END OF FILE**********/
