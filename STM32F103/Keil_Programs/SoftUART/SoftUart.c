/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name: SoftUart.c                                                      */
/* Autor:     Jonas Berger                                                    */
/* Version:   V1.00                                                           */
/* Date:       06/04/2019                                                     */
/* Description: Serielle Ausgabe über PortPin (SoftUart)                      */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#include <stm32f10x.h>

#define GPIOB_ODR GPIOB_BASE + 3*sizeof(uint32_t)   // Calc peripheral address GPIOB ODR
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))  
#define SER_TXD  *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,8)))  // PB8

/******************************************************************************/
/*            U N T E R P R O G R A M M:    init_leds_switches                */
/*                                                                            */
/* Aufgabe:   Initialisiert Portleitungen für LED / Schalterplatine           */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void initPorts(void) 
{
  int temp;

  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;       // enable clock for GPIOA (APB2 Peripheral clock enable register)
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;       // enable clock for GPIOB  (APB2 Peripheral clock enable register)

  // Schalter auf Led /Schalterplatine
  temp = GPIOA->CRL;
  temp &= 0x00000000;  // PA7-PA0 Konfigurationsbits löschen
  temp |= 0x88888888;  // PA7-PA0 als Input definieren
  GPIOA->CRL = temp;
  GPIOA->ODR |= 0x00FF;    // PA7-PA0 internen Pull Up aktivieren

  //LEDs auf Led / Schalterplatine
  temp = GPIOB->CRH;
  temp &= 0x00000000;  // PB15-PB8 Konfigurationsbits löschen  
  temp |= 0x33333333;  // PB15-PB8 als Push Pull Output definieren
  GPIOB->CRH = temp;
} 
/******************************************************************************/
/*            U N T E R P R O G R A M M:    wait_10us                         */
/*                                                                            */
/* Aufgabe:   Wartet 10µs                                                     */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void wait_10us()
{
  for(int i = 0; i < 16; i++);
}


/******************************************************************************/
/*            U N T E R P R O G R A M M:    wait                              */
/*                                                                            */
/* Aufgabe:   Wartet 100µs                                                    */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void wait() 
{
  for(int i = 0; i<10; i++) 
  {
    wait_10us();
  }
}

/******************************************************************************/
/*                                                                            */
/*                 Ausgabe eines Zeichens  auf SOFTUART                       */
/*                                                                            */
/******************************************************************************/
void v24_out(char c) 
{
  char i;

  SER_TXD = 0; // Sende Startbit
  wait();
  for (i = 0; i<8; i++)  // Sende 8 Datenbit
  {
    SER_TXD = (c&0x01)?1:0;
     wait();  
     c = c>>1;  // Nächstes Datenbit
  }
  SER_TXD = 1;
  wait();  // Sende Stoppbit
}

/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main (void) 
{
  char  *t = "DIC - 4ABHEL\n"; 

  initPorts();
  
  SER_TXD = 1;    // V24 Ruhezustand
  while(*t)       // Ausgabe des gesamten Texts
  {
    v24_out(*t++);
  }

  while(1);  // Endlosschleife
}  
