/******************************************************************************/
/* File Name:   SYS_CLK_STM32L432XX.c                                         */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        28.09.2021                                                    */
/* Description: SYS_CLK_STM32L432XX Library                                   */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define SYS_CLK_STM32L432XX_MOD

/*_______________________________ INCLUDES ___________________________________*/
#include "SYS_CLK_STM32L432XX.h"

/*______________________________ SUBROUTINES _________________________________*/
/******************************************************************************/
/*            SUBROUTINE:    set_clock_80MHz()                                */
/*                                                                            */
/* Aufgabe:   set SystemCoreClock to 80 MHz                                   */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void set_clock_80MHz(void)
{
	FLASH->ACR |= 1<<2;

	RCC->CR |= 1<<3;

	RCC->PLLCFGR &= ~(1<<12);
	RCC->PLLCFGR |= ((1<<13) | (1<<11)); // PLLN = 40
	RCC->PLLCFGR |= (1<<0); // PLLSRC = MSI

	RCC->PLLSAI1CFGR |= (1<<25); //PLLSAI1R = 4
	RCC->PLLSAI1CFGR &= ~(1<<12);
	RCC->PLLSAI1CFGR |= ((1<<8) | (1<<9) | (1<<11) | (1<<13)); //PLLSAI1N = 43

	RCC->CR |= (1<<24); // Enable: PLL
	while(((RCC->CR) & (1<<25)) == 0); //Wait till PLL is ready

	RCC->CR |= (1<<26); // Enable: PLLSAI1
	while(((RCC->CR) & (1<<27)) == 0); // Wait till PLLSAI1 is ready

	RCC->PLLCFGR |= (1<<24); // Enable: PLLREN Goes into System Clock 
	RCC->PLLSAI1CFGR |= (1<<24);  //Enable: PLLSAI1REN Goes into ADC

	RCC->CFGR |= ((1<<1) | (1<<0));
	
	SystemCoreClockUpdate(); // update SystemCoreClock-variable
}
