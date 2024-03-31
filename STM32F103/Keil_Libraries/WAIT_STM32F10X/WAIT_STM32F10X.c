/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   WAIT_STM32F10X.c                                              */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        07.05.2020                                                    */
/* Description: Warte x us bzw x ms ab                                        */
/*              Header-File stellt defines und Prototypen zur Verfuegung      */
/*              Befehle sind hier ausprogrammiert                             */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define WAIT_STM32F10X_MOD
#include "WAIT_STM32F10X.h"

/*________________________ Globale Variables _________________________________*/
static __IO uint32_t usTicks; //For store tick counts in us

/*__________________________ SUB-ROUTINES ____________________________________*/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    SysTick_Handler()                 */
/*                                                                            */
/* Aufgabe:   SysTick_Handler function will be called every 1 us              */
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
/*            U N T E R P R O G R A M M:    wait_init()                       */
/*                                                                            */
/* Aufgabe:   init Wait                                                       */
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
/*            U N T E R P R O G R A M M:    wait_us()                         */
/*                                                                            */
/* Aufgabe:   wartet x us ab                                                  */
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
/*            U N T E R P R O G R A M M:    wait_ms()                         */
/*                                                                            */
/* Aufgabe:   wartet x ms ab                                                  */
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
