/******************************************************************************/
/* File Name:   DIL_POTI.c                                                    */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        07.05.2020                                                    */
/* Description: get DIL potetntiometer value                                  */
/******************************************************************************/
/* History: 	V1.00  creation                                                 */
/******************************************************************************/
#define DIL_POTI_MOD
#include "DIL_POTI.h"

/*___________________________ SUBROUTINES ____________________________________*/
/******************************************************************************/
/*               SUBROUTINE:    poti_convert()                                */
/*                                                                            */
/* Purpose: delivers value of Portline PC4 (ADC1 Kanal 14)                    */
/* Input:   none                                                              */
/* return:  converted value (12Bit right aligned)                             */
/******************************************************************************/
unsigned short int poti_convert()
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPCEN; // enable periperal clock for ADC1
	GPIOC->CRL &= ~0x0F0000; // set PC4 as analog input
  
	ADC1->CR2 = 0x000E0001; // right data alignment, SWSTART Conv.
													// enable ADC (ADON), Single Mode
	
	ADC1->SMPR1 |= (0x5 << 4*3); //set sample time ADC1 channel 14 (55,5 cycles)
	ADC1->SQR1 = 0x00000000; //only one conversion item in regular sequence
	ADC1->SQR2 = 0x00000000;
	ADC1->SQR3 = 0x00000000|0xE;//set channel as first in regular conv. sequence
	ADC1->CR2 |= 0x00500000;//start conversion by software for regular sequence
	
	while((ADC1->SR & 0x2) == 0);//Wait till conversion is finished (EOC=1)
	
	return(ADC1->DR); //deliver converted value of channel
}
