/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name: motor_anst.c                                                    */
/* Autor:     Jonas Berger                                                    */
/* Version:   V1.00                                                           */
/* Date:      07/07/2019                                                      */
/* Description: Full Step Arduino-Schrittmotoransteuerung (28BYJ-48)          */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#include <armv10_std.h>

/*------------------------- Function Prototypes ------------------------------*/
void init_ports(void);
void next_step(int dir);
void waitms(int ms);

/******************************************************************************/
/*                   init_ports(void)                                         */ 
/* Funktion:                                                                  */    
/*   initialisiert GPIO Port A                                                */
/******************************************************************************/
void init_ports(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  GPIOA->CRL &= ~0xFFFF;
  GPIOA->CRL |= 0x3333;
}

/******************************************************************************/
/*                       next_step(int dir)                                   */ 
/* Funktion:                                                                  */    
/*   UEbergang vom vorherigen Schritt zum naechsten Schritt                   */
/* UEbergabeparameter: "dir" vom Typ integer:                                 */
/*                         0 ... Vorwaertsschritt                             */
/*                         1 ... Rueckwaertsschritt                           */
/******************************************************************************/
void next_step(int dir)
{
  //const char half_step[8] = {0x8,0xA,0x2,0x6,0x4,0x5,0x1,0x9};
  const char full_step[4] = {0xC,0x6,0x3,0x9};
  static int pos=0; //static damit im Unterprogramm nicht pos zurueckgesetzt wird
  
  if(!dir)
  {
    pos++;
    if(pos==4)
    {
      pos=0;
    }
  }
  else
  {
    pos--;
    if(pos==-1)
    {
      pos=3;
    }
  }
  GPIOA->ODR &= ~0x0F;
  GPIOA->ODR |= full_step[pos];
}

/******************************************************************************/
/*                       wait_ms(int ms)                                      */ 
/* Funktion:                                                                  */    
/*   Warteschleife in ms                                                      */
/* UEbergabeparameter: "ms" vom Typ integer:                                  */
/******************************************************************************/
void waitms(int ms)
{
  for(int i = 0; i < ms; i++)
  for(int j = 0; j < 8000; j++);; //8000 ist 1ms
}

/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main(void)
{
  int dir,ADC;
  init_ports();
  
  dir = 0;
  
  while(1)
  {
    next_step(dir);
    
    ADC = (adc1_convert(14)/10); //Wert von 0-4095; Channel 14 entspricht DIL-Poti
    if(ADC < 10) ADC = 10;
    if(ADC > 400) ADC = 400;
    waitms(ADC);
  }
}
