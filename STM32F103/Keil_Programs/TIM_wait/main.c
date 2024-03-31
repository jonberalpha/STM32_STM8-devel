/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   TIM_wait.c                                                    */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        15.09.2019                                                    */
/* Description: wait mit Timer                                                */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/

/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include "stm32f10x.h"

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/
#define CONST 1000 //1 s
/*------------------------------ Bit Banding ---------------------------------*/
#define GPIOB_ODR GPIOB_BASE + 3*sizeof(uint32_t)   // Calc peripheral address GPIOB ODR

// Calc Bit Band Adress from peripheral address: a = peripheral address b = Bit number
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))
#define LED0  *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,8)))  // PB8

/*------------------------------------------------------------------------------
 * Function Prototypes
 *----------------------------------------------------------------------------*/
void initPorts(void);
void configTIM1(void);
void initNVIC(char position, char priority); 

/*------------------------------------------------------------------------------
 * Global Static Variables
 *----------------------------------------------------------------------------*/
static volatile unsigned int toggle_1ms;

/******************************************************************************/
/*           Interrupt Service Routine  Timer1 (General Purpose Timer)        */
/******************************************************************************/
void TIM1_UP_IRQHandler(void)  //Timer 1, ISR alle 1 ms aufgerufen
{
  TIM1->SR &= ~0x01;   //Interrupt pending bit loeschen (Verhinderung eines nochmaligen Interrupts)
  
  toggle_1ms = 1;
}

/******************************************************************************/
/*                   NVIC_init(char position, char priority)                  */ 
/* Funktion:                                                                  */    
/*   UEbernimmt die vollständige initialisierung eines Interrupts  im Nested  */
/*   vectored Interrupt controller (Prioritaet setzen, Ausloesen verhindern,  */
/*   Interrupt enablen)                                                       */
/* UEbergabeparameter: "position" = 0-67 (Nummer des Interrupts)              */
/*                     "priority": 0-15 (Priorität des Interrupts)            */
/******************************************************************************/
void initNVIC(char position, char priority) 
{  
  NVIC->IP[position]=(priority<<4);  //Interrupt priority register: Setzen der Interrupt Prioritaet
  NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Clear Pendig Register: Verhindert, dass der Interrupt auslöst sobald er enabled wird 
  NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Set Enable Register: Enable interrupt
}

/******************************************************************************/
/*               Initialization Timer1 (General Purpose Timer)                */
/******************************************************************************/
void configTIM1(void)
{  
  /*--------------------------- Timer 1 konfigurieren ------------------------*/
  RCC->APB2ENR |= 0x0800;  //Timer 1 Clock enable
  TIM1->SMCR &= 0x0;    //Timer 1 Clock Selection: CK_INT wird verwendet
  TIM1->CR1  &= 0x0;    // Auswahl des Timer Modus: Upcounter --> Zählt: von 0 bis zum Wert des Autoreload-Registers
  
  TIM1->PSC = 49;       //Wert des prescalers (Taktverminderung)  -- damit er genau 1 ms laueft
  TIM1->ARR = 1600;     //Auto-Reload Wert = Maximaler Zaehlerstand des Upcounters
  TIM1->RCR = 0;       //Repetition Counter deaktivieren

  /*--------------------- Interrupt Timer 1 konfigurieren ---------------------*/
  TIM1->DIER |= 0x1;           //enable Interrupt bei einem UEV (UEberlauf / Unterlauf)
  initNVIC(TIM1_UP_IRQn,2);   //Enable Timer 1 Update Interrupt, Priority 2

  /*-------------------------- Timer 1 Starten -------------------------------*/
  TIM1->CR1 |= 0x1;   //Counter-Enable bit setzen
}

/******************************************************************************/
/*                         Initialization GPIOs                               */
/******************************************************************************/
void initPorts(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  GPIOB->CRH &= ~0xF;
  GPIOB->CRH |= 0x3;
}

/*------------------------------------------------------------------------------
 * Hauptprogramm
 *----------------------------------------------------------------------------*/
int main(void)
{
  initPorts();
  configTIM1();    //Config TIM1
  int counter = 0;
  
  while(1)
  {
    //run stuff which should run all the time
    if(toggle_1ms)
    {
      toggle_1ms = 0;
      if(counter == CONST)
      {
        counter = 0;
        LED0 = ~LED0;
      }
      counter++;
    }
  }
}
