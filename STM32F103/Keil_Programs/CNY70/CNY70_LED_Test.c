/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   CNY70_LED_Test.c                                              */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        10.03.2018                                                    */
/* Description: LED mit digitalisierten Sensor ansteuern                      */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#include <stm32f10x.h>

/*______________________________ BITBANDING __________________________________*/
#define GPIOB_ODR GPIOB_BASE + 3*sizeof(uint32_t) //Calc peripheral address GPIOB ODR
#define GPIOA_IDR GPIOA_BASE + 2*sizeof(uint32_t) //Calc peripheral address GPIOA IDR
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))  
#define LED   *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,8)))   //LED/PB8
#define SENSOR  *((volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,0))) //SENSOR/PA0

/*___________________________ FUNCTION PROTOTYPES______________________________*/
void init_ports(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    init_ports                        */
/*                                                                            */
/* Aufgabe:   Initialisiert Portleitungen für den Sensor und die LED          */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void init_ports(void)
{
  int temp;

  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;       // enable clock for GPIOA (APB2 Peripheral clock enable register)
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;       // enable clock for GPIOB  (APB2 Peripheral clock enable register)

  //init Sensor
  temp = GPIOA->CRL;
  temp &= 0x00000000;  // PA0 Konfigurationsbits loeschen
  temp |= 0x00000008;  // PA0 als Input definieren
  GPIOA->CRL = temp;
  GPIOA->ODR |= 0x0001; // PA0 internen Pull Up aktivieren (ODR)

  //init LED
  temp = GPIOB->CRH;
  temp &= 0x00000000;  // PB8 Konfigurationsbits löschen  
  temp |= 0x00000003;  // PB8 als Push Pull Output definieren
  GPIOB->CRH = temp;   
} 

/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main (void) 
{
  init_ports();
  
  do
  {
    if(SENSOR==0)
    {
      LED=1;
    }
    else
    {
      LED=0;
    }
  }
  while(1);
}
