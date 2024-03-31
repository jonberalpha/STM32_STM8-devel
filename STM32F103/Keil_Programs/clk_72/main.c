/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:         main.c                                                  */
/* Autor:             Berger Jonas                                            */
/* Version:           V1.00                                                   */
/* Date:(last change) 12.05.2020                                              */
/* Description: set_clock_72MHz                                               */
/******************************************************************************/
/* History:   V1.00  creation                                                  */
/******************************************************************************/
#include "stm32f10x.h"

void set_clock_72MHz(void) 
{
  FLASH->ACR = 0x12; //Set Flash latency (2 waitstates)
  RCC->CR |= RCC_CR_HSEON; //HSE on
  while ((RCC->CR & RCC_CR_HSERDY) == 0); // Wait for HSERDY=1 (HSE is ready)
  RCC->CFGR |= RCC_CFGR_PLLMULL9; // 9 mal 8 = 72 MHz (SYSCLK)
  RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6; //ADCCLK = SYSCLK/6 (APB2 PRESCALER=1)
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // PCLK1(APB1)=SYSCLK/2 (HCLK=SYSCLK)
  RCC->CFGR |= RCC_CFGR_PLLSRC; //PLL = SYSCLK,HCLK=SYSCLK,da AHB PRESCALER=1)
  RCC->CR |= RCC_CR_PLLON; //PLL on
  while ((RCC->CR & RCC_CR_PLLRDY) == 0); // Wait for PLLRDY=1 (PLL is ready)
  //RCC->CFGR |= RCC_CFGR_SW_HSE; //HSE = Systemclock
  RCC->CFGR |= RCC_CFGR_SW_PLL; //PLL = Systemclock
  while ((RCC->CFGR & RCC_CFGR_SWS_PLL) == 0);
  /* Wait till SYSCLK is stabilized (depending on selected clock) */
  while ((RCC->CFGR & RCC_CFGR_SWS) != ((RCC->CFGR<<2) & RCC_CFGR_SWS));
  // end of stm32_ClockSetup
  RCC->BDCR |=RCC_BDCR_LSEON; //32kHz für RTC siehe AN2821 Reference Manual 448ff/1072
  SystemCoreClockUpdate(); update SystemCoreClock-variable
}
/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main(void) 
{
  set_clock_72MHz();
  
  while(1);
}
