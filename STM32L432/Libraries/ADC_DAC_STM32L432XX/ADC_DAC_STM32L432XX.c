/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   ADC_DAC_STM32L432XX.c                                         */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        10.10.2021                                                    */
/* Description: ADC_DAC_STM32L432XX Library                                   */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define ADC_DAC_STM32L432XX_MOD

/*_______________________________ INCLUDES ___________________________________*/
#include "ADC_DAC_STM32L432XX.h"

/*_____________________________ UNTERPROGRAMME _______________________________*/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    _adc_configure()                  */
/*                                                                            */
/* Aufgabe:   configure ADC                                                   */
/* Input:     (uint8_t) - portPin (PA0 - PA7; channel 5 - channel 12)         */
/* return:    (uint8_t) - 0 upon success, >0 otherwise                        */
/******************************************************************************/
uint8_t _adc_configure(uint8_t portPin)
{
  // All ADC-channels:
  // PA0 ... ADC1 channel 5
  // PA1 ... ADC1 channel 6
  // PA2 ... ADC1 channel 7
  // PA3 ... ADC1 channel 8
  // PA4 ... ADC1 channel 9  !!! confilicting with DAC1 channel 1
  // PA5 ... ADC1 channel 10 !!! confilicting with DAC1 channel 2
  // PA6 ... ADC1 channel 11
  // PA7 ... ADC1 channel 12 | ClickBoard Button
  
  uint8_t ret = 0;
  
  RCC->CCIPR |= RCC_CCIPR_ADCSEL; // set sysclk as adc clock
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable peripheral clock for GPIOA
  GPIOA->MODER |= (0x3 << portPin); // set PAX in analog mode
  
  RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;  //enabele peripheral clock for ADC1
  ADC1->CR &= ~ADC_CR_DEEPPWD; // disable Deep sleep mode
  ADC1->CR |= ADC_CR_ADVREGEN; // enable Voltage regulator
  ADC1->CFGR |= ADC_CFGR_JQDIS; // disable injected qeue disable
  ADC1->CFGR &= ~(ADC_CFGR_RES | ADC_CFGR_ALIGN | ADC_CFGR_CONT); // right data alignment, SWSTART Conv., Single Mode
  
  return ret;
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    _adc_getval()                     */
/*                                                                            */
/* Aufgabe:   convert voltage to value from given portPin                     */
/* Input:     (uint16_t *) - Pointer to pValue (0 - 4095)                     */
/*            (uint8_t)    - portPin (PA0 - PA7; channel 5 - channel 12)      */
/* return:    (uint8_t)    - 0 upon success, >0 otherwise                     */
/******************************************************************************/
uint8_t _adc_getval(uint16_t *pValue, uint8_t portPin)
{
  uint8_t ret = 0;
  
  //                   ADC_IN-Pins PORTA:  A0,A1,A2,A3,A4,A5,A6,A7
  const int ADC1_channel_lookup_table[] = { 5, 6, 7, 8, 9,10,11,12};
  int channel = ADC1_channel_lookup_table[portPin]; // initialize channel
  
  if(channel <= 9)
  {
    ADC1->SMPR1 &= ~(0x7 << channel*3); // set sample time ADC1 channel x (2,5 cycles)
  }
  else
  {
    ADC1->SMPR2 &= ~(0x7 << (channel-10)*3); // set sample time ADC1 channel x (2,5 cycles)
  }
  
  ADC1->SQR1 &= ~0xF; // single conversion in L register, set channel as first in regular conv. sequence
  ADC1->SQR1 |= (channel << 0x6); //only one conversion item in regular sequence
  ADC1->SQR2 = 0x00000000;
  ADC1->SQR3 = 0x00000000;
  
  ADC1->CR |= ADC_CR_ADEN; // enable ADC (ADON)
  while((ADC1->ISR & ADC_ISR_ADRDY) == 0); // Wait until ADRDY=1
  
  ADC1->CR |= ADC_CR_ADSTART; //start conversion by software in regular sequence
  while((ADC1->ISR & ADC_ISR_EOC) == 0); // Wait till conversion is finished (EOC=1)
  
  *pValue = ADC1->DR;
  
  return ret;
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    _dac_setval()                     */
/*                                                                            */
/* Aufgabe:   output value on channel 1 (PA4) or channel 2 (PA5)              */
/* Input:     (uint16_t) - value (0 - 4095)                                   */
/*            (uint8_t)  - portPin (channel 1/PA4; channel 2/PA5)             */
/* return:    (uint8_t)  - 0 upon success, >0 otherwise                       */
/******************************************************************************/
uint8_t _dac_setval(uint16_t value, uint8_t portPin)
{
  // All DAC-channels:
  // PA4 ... DAC1 channel 1 !!! confilicting with ADC1 channel 9
  // PA5 ... DAC1 channel 2 !!! confilicting with ADC1 channel 10
  
  uint8_t ret = 0;
  
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable peripheral clock for GPIOA
  GPIOA->MODER |= (0x3 << portPin); // set PAX in analog mode
  
  RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN; // enable peripheral clock
  
  if(portPin == 5) // PA5/channel 2
  {
    DAC1->CR &= ~(0x1<<(2+16)); // no trigger
    DAC1->SWTRIGR &= ~(0x1<<1); // no software trigger
    DAC1->CR &= ~(0x3<<(6+16)); // no wave generation
    DAC1->MCR &= ~(0x3<<16); // normal mode: DAC Channel 2 is connected to external pin with Buffer enabled
    //DAC1->MCR |= (0x2<<16); // no buffer because you cant get voltages close to 0V
    //DAC1->MCR |= (0x3<<16); // on chip peripherals, no buffer because you cant get voltages close to 0V
    
    DAC1->CR |= (0x1<<16); // enable DAC1 channel 2
  }
  else // PA4/channel 1 default
  {
    DAC1->CR &= ~(0x1<<2); // no trigger
    DAC1->SWTRIGR &= ~(0x1<<0); // no software trigger
    DAC1->CR &= ~(0x3<<6); // no wave generation
    DAC1->MCR &= ~(0x3<<0); // normal mode: DAC Channel 1 is connected to external pin with Buffer enabled
    //DAC1->MCR |= (0x2<<0); // no buffer because you cant get voltages close to 0V
    //DAC1->MCR |= (0x3<<0); // on chip peripherals, no buffer because you cant get voltages close to 0V
    
    DAC1->CR |= (0x1<<0); // enable DAC1 channel 1
  }
  
  if(value > 4095) // limit value-variable
  {
    value = 4095;
  }
  
  if(portPin == 5) // PA5/channel 2
  {
    DAC1->DHR12R2 = value; // DR reg of channel 2
  }
  else // PA4/channel 1 default
  {
    DAC1->DHR12R1 = value; // DR reg of channel 1
  }
  
  return ret;
}
