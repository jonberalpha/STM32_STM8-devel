/******************************************************************************/
/* File Name:   TIMEOUT_STM32L432XX.c                                         */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        28.09.2021                                                    */
/* Description: TIMEOUT_STM32L432XX Library                                   */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define TIMEOUT_STM32L432XX_MOD

/*_______________________________ INCLUDES ___________________________________*/
#include "TIMEOUT_STM32L432XX.h"

/*_______________________ STATIC GLOBAL VARIABLES ____________________________*/
static int sw = 0;
static int timticks = 0;
static void (*funcp)();

/*___________________________ PRIVATE PROTOTYPEN _____________________________*/
static void set_clock_80MHz(void);
static void TIM1_init(void);
static void TIM2_init(void);
static void delay_us(uint32_t us);
static void delay_ms(uint32_t ms);

/*__________________________________ ISRs ____________________________________*/
void TIM1_UP_TIM16_IRQHandler(void) // for blocking-timeout
{
	TIM1->SR &= ~TIM_SR_UIF; // clear pending bit to avoid recalling TIM_ISR
	
	timticks++;
}

void TIM2_IRQHandler(void) // for nonblocking-timeout
{
	TIM2->SR &= ~TIM_SR_UIF; // clear pending bit to avoid recalling TIM_ISR
	
	if(sw >= 1) // switch, da beim enablen gleich die ISR ausgefuehrt wird
	{
		// execute function wich shall be delayed
		funcp(); // executes whatever function assigned early on
		sw = 2;
		TIM2->CR1 &= ~TIM_CR1_CEN; // disable Timer 2
	}
	else
	{
		sw++;
	}
}

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
/*            SUBROUTINE:    timeout_init()                                   */
/*                                                                            */
/* Purpose:   initialize Timer1 (blocking) & Timer2 (nonblocking)             */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void timeout_init(void)
{
	set_clock_80MHz();
	TIM1_init();
	TIM2_init();
}

/******************************************************************************/
/*            SUBROUTINE:    TIM1_init()                                      */
/*                                                                            */
/* Purpose:   initialize Timer1                                               */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
static void TIM1_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // enable TIMER 1 - Clock
	SystemCoreClockUpdate(); // refresh SystemCoreClock-variable
	
	TIM1->PSC = SystemCoreClock/1000000 - 1; // prescale 80MHz to 1MHz 80-1
	TIM1->ARR = 1; // every count = 1µs
	TIM1->CR1 |= TIM_CR1_URS;   // set URS-flag: only overflow/underflow generates update interrupt
	TIM1->DIER |= TIM_DIER_UIE; // enable Update interrupt
	
  NVIC_SetPriority(TIM1_UP_TIM16_IRQn,8); // less priority than timer 2
	NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
}

/******************************************************************************/
/*            SUBROUTINE:    TIM2_init()                                      */
/*                                                                            */
/* Purpose:   initialize Timer2                                               */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
static void TIM2_init(void)
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // enable TIMER 2 - Clock
	SystemCoreClockUpdate(); // refresh SystemCoreClock-variable
	
	TIM2->PSC = SystemCoreClock/1000000 - 1; // prescale 80MHz to 1MHz 80-1
	TIM2->ARR = 0xFFFFFFFF - 1; // ARR Wert maximal 32bit Register ... wird vom user sowieso veraendert
	TIM2->CR1 |= TIM_CR1_URS;   // set URS-flag: only overflow/underflow generates update interrupt
	TIM2->DIER |= TIM_DIER_UIE; // enable Update interrupt
	
  NVIC_SetPriority(TIM2_IRQn,7);
	NVIC_EnableIRQ(TIM2_IRQn);
}

/******************************************************************************/
/*            SUBROUTINE:    delay_us()                                       */
/*                                                                            */
/* Purpose:   wait given us                                                   */
/* Input:     (uint32_t) - Time in us                                         */
/* return:                                                                    */
/******************************************************************************/
static void delay_us(uint32_t us)
{
	TIM1->CR1 |= TIM_CR1_CEN; // enable Timer 1
	timticks = 0;
	while(timticks < us) // blocking loop
	{
		__WFI(); // sleep until interrupt occur; in this case wait until Timerinterrupt occurs
	}
	TIM1->CR1 &= ~TIM_CR1_CEN; // after loop disable Timer 1
}

/******************************************************************************/
/*            SUBROUTINE:    delay_ms()                                       */
/*                                                                            */
/* Purpose:   wait given ms                                                   */
/* Input:     (uint32_t) - Time in ms                                         */
/* return:                                                                    */
/******************************************************************************/
static void delay_ms(uint32_t ms)
{
	TIM1->CR1 |= TIM_CR1_CEN; // enable Timer 1
	timticks = 0;
	while(timticks < (ms*1000)) // blocking loop
	{
		__WFI(); // sleep until interrupt occur; in this case wait until Timerinterrupt occurs
	}
	TIM1->CR1 &= ~TIM_CR1_CEN; // after loop disable Timer 1
}

/******************************************************************************/
/*            SUBROUTINE:    _tim_timeout_blocking()                          */
/*                                                                            */
/* Purpose:   wait given time blocking                                        */
/* Input:     (uint32_t) - time, (int) - MS or US                             */
/* return:                                                                    */
/******************************************************************************/
void _tim_timeout_blocking(uint32_t time, int unit)
{
	timticks = 0;
	
	if(unit == US)
	{
		delay_us(time);
	}
	if(unit == MS)
	{
		delay_ms(time);
	}
}

/******************************************************************************/
/*            SUBROUTINE:    _tim_timeout_nonblocking_with_callback()         */
/*                                                                            */
/* Purpose:   execute function non blocking after given time                  */
/* Input:     (uint32_t) - time, (uint16_t) - timeunit, adress of pointer2func*/
/* return:                                                                    */
/******************************************************************************/
void _tim_timeout_nonblocking_with_callback(uint32_t time, uint16_t unit, void (*func)())
{
  sw = 0;
	funcp = func;

	if(unit == US)
	{
		TIM2->PSC = SystemCoreClock/1000000 - 1; // clock auf 1us reduziert: 80 - 1
		TIM2->ARR = time; // every 10 counts 1ms (fsys = 80MHz) -> ISR; given MS * 10 because every 10 count equals 1ms
	}
	if(unit == MS)
	{
		// change PSC and ARR to fit MS
		TIM2->PSC = SystemCoreClock/10000 - 1; // clock auf 10kHz(=100us) reduziert: 80MHZ/10000 -> PSC =8000-1
		TIM2->ARR = 10*time; // alle 10*100us = 1ms (fsys = 80MHz)
	}
	
	TIM2->CR1 |= TIM_CR1_CEN; // enable Timer 2
}
