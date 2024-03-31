/******************************************************************************/
/*   (C) Copyright - PRIVATE   2020 All rights reserved. AUSTRIA              */
/*                                                                            */
/* File Name:   ADC_STM32F10X.c                                               */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        23.07.2020                                                    */
/* Description: Get ADC Value on ch0 - ch9 of ADC1                            */
/*              Header-File stellt defines und Prototypen zur Verfuegung      */
/*              Befehle sind hier ausprogrammiert                             */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define ADC_STM32F10X_MOD
#include "ADC_STM32F10X.h"

/*_______________________________ SUBROUTINES ________________________________*/
/******************************************************************************/
/*               U N T E R P R O G R A M M:    adc_convert()                  */
/*                                                                            */
/* Aufgabe: delivers value of ADC1 Kanal x                                    */
/* Input:   int channel                                                       */
/* return:  converted value (12Bit right aligned)                             */
/******************************************************************************/
unsigned short int adc_convert(int channel)
{
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;  //enabele peripheral clock for ADC1
  
  channel = (channel > 9)?9:channel;   //channel Vorverarbeitung
  
  if(channel < 8)
  {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // enable peripheral clock
    GPIOA->CRL &= ~(0x0F << channel*4); // set PAX as analog input
  }
  else
  {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // enable peripheral clock
    GPIOB->CRL &= ~(0x0F << (channel-8)*4); // set PBX as analog input
  }
  
  ADC1->CR2 = 0x000E0001;  // right data alignment, SWSTART Conv.
                           // enable ADC (ADON), Single Mode
  
  ADC1->SMPR1 |= (0x5 << channel*3); //set sample time ADC1 channel 14 (55,5 cycles)
  ADC1->SQR1 = 0x00000000; //only one conversion item in regular sequence
  ADC1->SQR2 = 0x00000000;
  ADC1->SQR3 = 0x00000000|channel; //set channel as first in regular conv. sequence
  ADC1->CR2 |= 0x00500000; //start conversion by software for regular sequence
  
  while ((ADC1->SR & 0x2) == 0);//Wait till conversion is finished (EOC=1)
  
  return(ADC1->DR); //deliver converted value of channel
}
