/******************************************************************************/
/* File Name:   ADC_POTI_STM32L432XX.c                                        */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        28.09.2021                                                    */
/* Description: ADC_POTI_STM32L432XX Library                                  */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define ADC_POTI_STM32L432XX_MOD

/*_______________________________ INCLUDES ___________________________________*/
#include "ADC_POTI_STM32L432XX.h"

/*______________________________ SUBROUTINES _________________________________*/
/******************************************************************************/
/*            SUBROUTINE:    adc_read_poti()                                  */
/*                                                                            */
/* Purpose:   read Poti-Value on PA7 (ADC1/channel 12)                        */
/* Input:                                                                     */
/* return:    (unsigned int) - poti value                                     */
/******************************************************************************/
unsigned int adc_read_poti(void)
{
	int pin = 7;
	int channel = 14;
	
	RCC->CCIPR |= RCC_CCIPR_ADCSEL; // set sysclk as adc clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable peripheral clock
	GPIOA->MODER |= (0x3 << pin); // set PAX in analog mode
	
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;  //enabele peripheral clock for ADC1
	ADC1->CR &= ~ADC_CR_DEEPPWD; // disable Deep sleep mode
	ADC1->CR |= ADC_CR_ADVREGEN; // enable Voltage regulator
	ADC1->CFGR |= ADC_CFGR_JQDIS; // disable injected qeue disable
	ADC1->CFGR &= ~(ADC_CFGR_RES | ADC_CFGR_ALIGN | ADC_CFGR_CONT); // right data alignment, SWSTART Conv., Single Mode
	
	if(channel <= 9)
	{
		ADC1->SMPR1 &= ~(0x7 << channel*3);//(0x5 << channel*3); // (3 cycles)
	}
	else
	{
		ADC1->SMPR2 &= ~(0x7 << (channel-10)*3);//(0x5 << channel*3);////(0x5 << channel*3); //set sample time ADC1 channel x (92,5 cycles)
	}
	
	ADC1->SQR1 &= ~0xF; // single conversion in L register, set channel as first in regular conv. sequence
	ADC1->SQR1 |= (channel << 0x6); //only one conversion item in regular sequence
	ADC1->SQR2 = 0x00000000;
	ADC1->SQR3 = 0x00000000;
		
	ADC1->CR |= ADC_CR_ADEN; // enable ADC (ADON)
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0); // Wait until ADRDY=1
	
	ADC1->CR |= ADC_CR_ADSTART; //start conversion by software for regular sequence
	while((ADC1->ISR & ADC_ISR_EOC) == 0);//Wait till conversion is finished (EOC=1)
	
	return(ADC1->DR); //deliver converted value of channel
}
