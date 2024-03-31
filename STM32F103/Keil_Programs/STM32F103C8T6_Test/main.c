/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   main.c                                                        */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        30.06.2020                                                    */
/* Description: LED Test on PA7                                               */
/******************************************************************************/
/* History:     V1.00  creation                                               */
/******************************************************************************/
#include "header.h"
#include <WAIT_STM32F10X.h>

/******************************************************************************/
/*                              PRIVATE PROTOTYPES                            */
/******************************************************************************/
static void init_ports(void);

/******************************************************************************/
/*                                 SUBROUTINES                                */
/******************************************************************************/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    init_ports()                      */
/*                                                                            */
/* Aufgabe:   initialize ports                                                */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
static void init_ports(void)
{
  //RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  //GPIOA->CRL &= ~0x0F0000000; //Konfig-Bits von PA7 auf 0 setzen
  //GPIOA->CRL |= 0x030000000; //PA7 auf GPOPP setzen
	
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  GPIOC->CRH &= ~0x000F00000; //Konfig-Bits von PC13 auf 0 setzen
  GPIOC->CRH |=  0x000300000; //PC13 auf GPOPP setzen
}

/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main(void) 
{
  init_ports();
  wait_init();
  
  while(1)
  {
    LEDC = 1;
    wait_ms(500);
    LEDC = 0;
    wait_ms(500);
  }
}
