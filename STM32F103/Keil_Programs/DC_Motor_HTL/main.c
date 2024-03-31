/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   DC_Motor.c                                                    */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        27.05.2018 (last change)                                      */
/* Description: Ansteuerung und Drehzahlbestimmung eines DC-Motors mit Uhr    */
/******************************************************************************/
/* History:     V1.00  creation                                               */
/******************************************************************************/
#include "stm32f10x.h"
#include "WAIT_STM32F10X.h"
#include "USART_STM32F10X.h"
#include "ADC_STM32F10X.h"
#include <stdio.h>

/*-------------------------- Function Prototypes -----------------------------*/
void init_ports(void);  //initialisiert uC-Ports
void TIM3_config(void); //Timer 3 config, fuer Motoransteuerung

/******************************************************************************/
/*                Initialization Timer 3 (General Purpose Timer)              */
/* Funktion:                                                                  */
/*   initialisiert Timer 3, der fuer die Motoransteuerung zustaendig ist      */
/*   CC4 CH4(PB1) als Output Compare verwendet ->INT=alle 100ms               */
/******************************************************************************/
void TIM3_config(void)
{  
  /*------------------------- Timer 3 konfigurieren --------------------------*/
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  //Timer 3 Clock enable
  TIM3->SMCR &= ~0x07; //Timer 3 Clock Selection: CLK_INT wird verwendet
  TIM3->CR1  |= 0x010; // Auswahl des Timer Modus: Downcounter -> Zaehlt: von ARR bis 0
  
  TIM3->PSC = 1; //Prescaler PSC->149 ARR->48000 => Ttim=100 ms
  TIM3->ARR = 8000; //Auto-Reload Wert (max 65536) = Maximaler Zaehlerstand des Downcounters
  
  /*--------------------- Interrupt Timer 3 konfigurieren --------------------*/
  TIM3->DIER |= 0x01; //enable Interrupt bei einem UEV (UEberlauf / Unterlauf)
  
  /*------------- Configure Channel 4(PB1) as Output Compare -----------------*/
  TIM3->CCR4 = 2000; //1/4 von ARR-Wert=20000 als Level festgelegt
  TIM3->CCMR2 |= 0x07800; //CC4S=00, OC4PE=1, OC4M=111
  TIM3->CCER |= 0x01000; //Enable CH4 als Output Compare
  
  /*---------------------------- Enable Timer 3 ------------------------------*/
  TIM3->CR1 |= 0x1; //Enable Timer 3
}

/******************************************************************************/
/*                   init_ports()                                             */ 
/* Funktion:                                                                  */    
/*   initialisiert uC-Ports  (PB9 as IC, PB1 as OC)                           */
/******************************************************************************/
void init_ports(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //GPIOB Clock enable
  
  GPIOB->CRL &= ~0x0F0; //PB1 Konfigurationsbit loeschen
  GPIOB->CRL |= 0x0B0;  //PB1 auf AFOPP fuer Output Compare: Motorantrieb
}

/******************************************************************************/
/*                              MAIN function                                 */
/******************************************************************************/
int main(void) 
{
  int ADC; //Wert des DIL-Poti fuer PWM-Einstellung
  
  wait_init();
  uart_init(9600);
  init_ports();  //initialisiert uC-Ports
  TIM3_config(); //Timer 3 config, fuer Motoransteuerung(Output Compare)
  
  while(1)
  {
    /*---------------- Motor PWM-Einstellung mit Poti --------------------*/
    ADC=adc_convert(8); //Wert von 0-4000; Channel 8 entspricht Poti
    //ADC=ADC*2; //Wert*2
    if(ADC<=400*2) //PWM von 100%->High und 0%->Low verhindern
    {
      ADC=400*2;
    }
    if(ADC>=3700*2) //PWM von 0%->High und 100%->Low verhindern
    {
      ADC=3700*2;
    }
    
    TIM3->CCR4=ADC; //ADC-Wert als Output Compare Level verwenden
  }

}
