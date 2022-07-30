/******************************************************************************/
/* File Name:   WAIT_STM32F10X.c                                              */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        07.05.2020                                                    */
/* Description: Wait for x us or x ms                                         */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define WAIT_STM32F10X_MOD
#include "WAIT_STM32F10X.h"

/*________________________ GLBOAL VARIABLES __________________________________*/
static __IO uint32_t usTicks; //Store tick counts in us

/*__________________________ SUBROUTINES _____________________________________*/
/******************************************************************************/
/*            SUBROUTINE:    SysTick_Handler()                                */
/*                                                                            */
/* Purpose:   SysTick_Handler function will be called every 1 us              */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void SysTick_Handler()
{
  if(usTicks != 0)
  {
    usTicks--;
  }
}

/******************************************************************************/
/*            SUBROUTINE:    wait_init()                                      */
/*                                                                            */
/* Purpose:   init Wait                                                       */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void wait_init()
{
  // Update SystemCoreClock value
  SystemCoreClockUpdate();
  // Configure the SysTick timer to overflow every 1 us
  SysTick_Config(SystemCoreClock / 1000000);
}

/******************************************************************************/
/*            SUBROUTINE:    wait_us()                                        */
/*                                                                            */
/* Purpose:   wartet x us ab                                                  */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void wait_us(uint32_t us)
{
  // Reload us value
  usTicks = us;
  // Wait until usTick reach zero
  while(usTicks);
}

/******************************************************************************/
/*            SUBROUTINE:    wait_ms()                                        */
/*                                                                            */
/* Purpose:   wartet x ms ab                                                  */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void wait_ms(uint32_t ms)
{
  // Wait until ms reach zero
  while(ms--)
  {
    // Delay 1ms
    wait_us(1000);
  }
}
