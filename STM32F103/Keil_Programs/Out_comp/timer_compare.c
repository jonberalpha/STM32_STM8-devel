/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   Timer_Compare_PWM.c                                           */
/* Autor:       Josef Reisinger                                               */
/* Version:     V1.00                                                         */
/* Date:        31/03/2012                                                    */
/* Description: LED Dimmer: Demoprogramm für Timer Compare                    */
/*              Funktionalitaet (PWM Out)                                     */ 
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#include "armv10_std.h"             

/*------------------------------ Function Prototypes -------------------------*/
static void TIM4_Config(void);
static void NVIC_init(char position, char priority); 

/*------------------------------ Static Variables-------------------------*/

/******************************************************************************/
/*                     Timer 4 Interrupt Service Routine                      */
/******************************************************************************/
void TIM4_IRQHandler(void){            //Timer 4 = PWM Output Mode, Channel 3
static char direction=1;              //Auf- / Abdimmen

  TIM4->SR &=~0x00FF;                     //Interrupt pending-bit löschen
  if(direction==0) {
    TIM4->CCR3++;                  //Längere High-Phase > heller
    if(TIM4->CCR3>=(TIM4->ARR/2)) {
      direction=1;              //Danach abdimmen
      }                    
    }
  else {
    TIM4->CCR3--;                 //Längere Low-Phase > dünkler
    if(TIM4->CCR3==0){
        direction=0;              //Danach aufdimmen
    }                    
    }  
  return;
}

/******************************************************************************/
/*                   NVIC_init(char position, char priority)                  */ 
/* Funktion:                                                                  */    
/*   Übernimmt die vollständige initialisierung eines Interrupts  im Nested   */
/*   vectored Interrupt controller (Priorität setzen, Auslösen verhindern,    */
/*   Interrupt enablen)                                                       */
/* Übergabeparameter: "position" = 0-67 (Nummer des Interrupts)               */
/*                    "priority": 0-15 (Priorität des Interrupts)             */
/******************************************************************************/
static void NVIC_init(char position, char priority) 
{  
  NVIC->IP[position]=(priority<<4);  //Interrupt priority register: Setzen der Interrupt Priorität
  NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Clear Pendig Register: Verhindert, dass der Interrupt auslöst sobald er enabled wird 
  NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Set Enable Register: Enable interrupt

} 

/******************************************************************************/
/*                     Initialization Timer4 (General Purpose Timer)          */
/******************************************************************************/
static void TIM4_Config(void)
{  
  /*-------------------------------------- Timer 4 konfigurieren ------------------------------------*/
  RCC->APB1ENR |= 0x0004;  //TIM4 Clock enable
  TIM4->SMCR = 0x0000;  //Slave Mode disabled - CK_INT wird verwendet
  TIM4->CR1 = 0x0000;      //Upcounter: Zählt von 0 bis zum Wert des Autoreload-Registers
    TIM4->CR1 |= 0x0080;  //ARPE-Bit setzen (Auto Reload Register buffered)

  /* ---------- T_INT = 126,26ns, Annahme: Presc = 0x00FF --> Auto Reload Wert = 0x00FF (=256) --> 8,27ms*/
  TIM4->PSC = 0xFF;    //Prescaler
  TIM4->ARR = 0xFF;    //Auto-Reload Register
  TIM4->DIER = 0x01;      // Enable Update Interrupt
  NVIC_init(TIM4_IRQn,1);        // Enable Interrupt Timer4 at NVIC (Priority 1)

    /*---------------Configure Compare Unit Timer 4: PWM Modus 1 auf Channel 3 -------------------------*/
  TIM4->CCER &=0xF0FF;         // Capture/Compare Enable Register: Disable Output Capture/Compare3 
  TIM4->CCR3 = (TIM4->ARR)/2;  // Vergleichswert Capture/Compare 3 laden (Hälfte des max Zaehlerstand)

  TIM4->CCMR2 |=0x0068;  // Capture/Compare Mode Register:
                          // Capture/Compare Kanal 3->PWM Mode1: Upcounter --> CNT < CounterVal: Ausgang=1, sonst 0 
  TIM4->CCMR2 &=0xFFFC;  //Als Capture/Compare Kanal 3 als Output definieren
                          
  TIM4->CCER |= 0x0100;   // Enable Output Capture/Compare3 --> Ausgang TIM4_CH3 auf 0 
   
}

/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main (void) 
{

  init_leds_switches();  // LED-Schalter Platine initialisieren (Schalter PA0-PA7, LED PB8-PB15

  GPIOB->CRH &=~ 0x0000000F;  //Bereich löschen
  GPIOB->CRH |=  0x0000000B;  //PB8(=LED0) in Mode Alternate Function Push Pull definieren => TIM4_CH3


  TIM4_Config();  // Timer 4 konfigurieren (PWM Modus auf Kanal 3)
  /*-------------------------- Timer 4 Starten -------------------------------*/
    TIM4->CR1 |= 0x0001;   //Counter-Enable bit für Timer 4 setzen

do {
  
} while (1==1);

}

