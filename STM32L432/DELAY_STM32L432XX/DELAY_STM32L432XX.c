/******************************************************************************/
/* File Name:   DELAY_STM32L432XX.c                                           */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        28.09.2021                                                    */
/* Description: DELAY_STM32L432XX Library                                     */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define DELAY_STM32L432XX_MOD

/*_______________________________ INCLUDES ___________________________________*/
#include "DELAY_STM32L432XX.h"

/*_______________________ STATIC GLOBAL VARIABLES ____________________________*/
static __IO uint32_t usTicks;

/*___________________________ PRIVATE PROTOTYPEN _____________________________*/
static void set_clock_80MHz(void);

/*_____________________________ SUBROUTINES __________________________________*/
/******************************************************************************/
/*            SUBROUTINE:    set_clock_80MHz()                                */
/*                                                                            */
/* Purpose:   set SystemCoreClock to 80 MHz                                   */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
static void set_clock_80MHz(void)
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

/******************************************************************************/
/*            SUBROUTINE:    delay_init()                                     */
/*                                                                            */
/* Purpose:   initialize SysTick-Timer                                        */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void delay_init(void)
{
	// set SystemCoreClock to 80MHz
	set_clock_80MHz();
	// Update SystemCoreClock value
	SystemCoreClockUpdate();
	// Configure the SysTick timer to overflow every 1 us
	SysTick_Config(SystemCoreClock/1000000);
}

/******************************************************************************/
/*            SUBROUTINE:    delay_us()                                       */
/*                                                                            */
/* Purpose:   wait given us                                                   */
/* Input:     (uint32_t) - Zeit in us                                         */
/* return:                                                                    */
/******************************************************************************/
void delay_us(uint32_t us)
{
	// Reload us value
	usTicks = us;
	// Wait until usTick reach zero
	while(usTicks);
}

/******************************************************************************/
/*            SUBROUTINE:    delay_ms()                                       */
/*                                                                            */
/* Purpose:   wait given ms                                                   */
/* Input:     (uint32_t) - Zeit in ms                                         */
/* return:                                                                    */
/******************************************************************************/
void delay_ms(uint32_t ms)
{
	// Wait until ms reach zero
	while(ms--)
	{
		// Delay 1ms
		delay_us(1000);
	}
}
