/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   motor_anst.c                                                  */
/* Autor:       Jonas Berger                                                  */
/* Version:     V1.00                                                         */
/* Date:        11/03/2012                                                    */
/* Description: Demoprogramm für Timer 1                                      */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#include <stm32f10x.h>
#include "WAIT_STM32F10X.h"

/*_______________________________ PROTOTYPES _________________________________*/
void init_ports(void);

/*------------------------------- BITBANDING ---------------------------------*/
#define GPIOB_ODR GPIOB_BASE + 3*sizeof(uint32_t) //Calc peripheral address GPIOB ODR
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4))) //Formel
#define PB7  *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,7))) //PB7

/******************************************************************************/
/*                   init_ports()                                             */
/* Funktion:                                                                  */
/*   initialisiert die GPIO Ports                                             */
/******************************************************************************/
void init_ports(void) 
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // enable clock for GPIOB  (APB2 Peripheral clock enable register)
  GPIOB->CRL &= ~0x0F0000000; // PB7 auf 
  GPIOB->CRL |= 0x070000000; // PB7 auf GPOOD | wr '1' is high imp -> 'z'; wr '0' is low
}

/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main(void) 
{
  wait_init();
  init_ports();
  
  while(1)
  {
    PB7 = 1; //high imp -> 'z'
    wait_ms(250);
    PB7 = 0; //pull to low
    wait_ms(250);
  }
}
