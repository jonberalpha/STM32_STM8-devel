/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name: main.c                                                          */
/* Autor:     Jonas Berger                                                    */
/* Version:   V1.00                                                           */
/* Date:      06/04/2019                                                      */
/* Description: Lauflicht mit Haupschalter und variabler Geschw. mit DIL-Poti */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/

/***********************************Includes***********************************/
#include <armv10_std.h>

/**********************************Prototypes**********************************/
void waitms(int ms);
void initPorts(void);
void setLeds(int value);
int getSwitches(void);

/******************************************************************************/
/*                   waitms()                                                 */ 
/* Funktion:                                                                  */    
/*   wartet die uebergebene Anzahl an ms                                      */
/******************************************************************************/
void waitms(int ms)
{
  int i,j;

  for(i = 0; i < ms; i++) 
  {
    for(j = 0; j < 1595; j++);
  }
} 

/******************************************************************************/
/*                   setLeds()                                                */ 
/* Funktion:                                                                  */    
/*   wartet die uebergebene Anzahl an ms                                      */
/******************************************************************************/
void setLeds(int value)
{
  GPIOB->ODR = (GPIOB->ODR & 0x00FF) | ((value & 0x00FF) <<8);
}

/******************************************************************************/
/*                   getSwitches()                                            */ 
/* Funktion:                                                                  */    
/*   wartet die uebergebene Anzahl an ms                                      */
/******************************************************************************/
int getSwitches(void)
{
  int temp;
  
  temp = GPIOA->IDR & 0x0FF;
  return ~temp; // Input softwaremaeszig invertieren
}

/******************************************************************************/
/*                   initPorts()                                              */ 
/* Funktion:                                                                  */    
/*   initialisiert die GPIO Ports                                             */
/******************************************************************************/
void initPorts(void) 
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // Clock
  
  // Switches
  GPIOA->CRL &= ~0xFFFFFFFF; // Config-Bit loeschen
  GPIOA->CRL |= 0x88888888;  // IPPU
  GPIOA->ODR |= 0x0FF;  // PU aktivieren
  
  // LEDs
  GPIOB->CRH &= 0x00;   // alle Config-Bits loeschen
  GPIOB->CRH |= 0x33333333;   // GPOPP
}

/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main(void) 
{  
  char Muster[9] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
  int i = 0;
  int help = 0;
  int ADC = 0;
  char lcdString[50];
  
  initPorts();
  lcd_init();
  lcd_clear();
  lcd_set_cursor(0,0);
  lcd_put_string("APPL. RUNNING...");
  
  while(1)
  {
    ADC = (adc1_convert(14)/10); //Wert von 0-4000; Channel 14 entspricht DIL-Poti
    if(ADC < 10) ADC = 10;
    
    sprintf(lcdString,"ADC-Wert=%d  ",ADC);
    lcd_set_cursor(1,0);
    lcd_put_string(lcdString);
    if((getSwitches() & 0x01) == 0x01) // Hauptschalter
    {
      setLeds(Muster[i]);
      waitms(ADC);
      
      if((i < 7)&&(help == 0))
      {
        i++;
      }
      else
      {
        help = 1;
      }
      
      if((i > 0)&&(help == 1))
      {
        i--;
      }
      else
      {
        help = 0;
      }
      
      setLeds(0x00);
    }
  }
}
