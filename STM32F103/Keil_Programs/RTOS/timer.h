/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   timer.c                                                       */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        05/04/2018                                                    */
/* Description: Timer                                                         */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include "stm32f10x.h"

/*------------------------------ Function Prototypes -------------------------*/
static void TIM1_Config(void);
static void NVIC_init(char position, char priority); 

/*------------------------------ Static Global Variables----------------------*/
static volatile int tick;

/******************************************************************************/
/*           Interrupt Service Routine  Timer1 (General Purpose Timer)        */
/******************************************************************************/
void TIM1_UP_IRQHandler(void)  //Timer 1, ISR alle 1 ms aufgerufen
{
  TIM1->SR &= ~0x1;   //Interrupt pending bit loeschen (Verhinderung eines nochmaligen Interrupt)
  
  tick = 1;
}

/******************************************************************************/
/*               Initialization Timer1 (General Purpose Timer)                */
/******************************************************************************/
static void TIM1_Config(void)
{  
  /*---------------------- Timer 1 konfigurieren -----------------------------*/
  RCC->APB2ENR |= 0x0800;  //Timer 1 Clock enable
  TIM1->SMCR = 0x0000;    //Timer 1 Clock Selection: CK_INT wird verwendet
  TIM1->CR1  = 0x0000;    // Auswahl des Timer Modus: Upcounter --> Zählt: von 0 bis zum Wert des Autoreload-Registers
  
  TIM1->PSC = 49;       //Wert des prescalers (Taktverminderung)  -- damit er genau 1 ms laueft
  TIM1->ARR = 1600;     //Auto-Reload Wert = Maximaler Zaehlerstand des Upcounters
  TIM1->RCR = 0;       //Repetition Counter deaktivieren

  /*--------------------- Interrupt Timer 1 konfigurieren ---------------------*/
  TIM1->DIER = 0x01;             //enable Interrupt bei einem UEV (UEberlauf / Unterlauf)
  NVIC_init(TIM1_UP_IRQn,2);     //Enable Timer 1 Update Interrupt, Priority 2

  /*-------------------------- Timer 1 Starten -------------------------------*/
  TIM1->CR1 |= 0x0001;   //Counter-Enable bit setzen
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
static void NVIC_init(char position, char priority) 
{  
  NVIC->IP[position]=(priority<<4);  //Interrupt priority register: Setzen der Interrupt Prioritaet
  NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Clear Pendig Register: Verhindert, dass der Interrupt auslöst sobald er enabled wird 
  NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Set Enable Register: Enable interrupt
}

#endif /* __TIMER_H */
