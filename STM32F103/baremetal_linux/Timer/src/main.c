/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   timer.c                                                       */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:         05/04/2018                                                   */
/* Description: Uhr  mit HSE 72MHz                                            */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#include <stm32f1xx.h>

/*------------------------------ Function Prototypes -------------------------*/
static void TIM1_Config(void);
void clk_init(void);

/*------------------------------ Static Variables-----------------------------*/
static int count = 0;

/******************************************************************************/
/*           Interrupt Service Routine  Timer1 (General Purpose Timer)        */
/******************************************************************************/
void TIM1_UP_IRQHandler(void)  //Timer 1, löst alle 1000ms aus
{
  TIM1->SR &= ~0x01;   //Interrupt pending bit löschen (Verhinderung eines nochmaligen Interrupt-auslösens)
  GPIOC->ODR ^= (1 << 13);
}

/******************************************************************************/
/*           Initialization Timer1 (General Purpose Timer)                    */
/******************************************************************************/
static void TIM1_Config(void)
{  
  /*---------------------- Timer 1 konfigurieren -----------------------*/
       
  RCC->APB2ENR |= 0x0800;  //Timer 1 Clock enable
  TIM1->SMCR = 0x0000;    //Timer 1 Clock Selection: CK_INT wird verwendet
  TIM1->CR1  = 0x0000;    // Auswahl des Timer Modus: Upcounter --> Zählt: von 0 bis zum Wert des Autoreload-Registers
  
  /* T_INT = 100ns, Annahme: Presc = 135 ---> Auto Reload Wert = 52801 (=0xCE41) --> 1s Update Event*/
  TIM1->PSC = 150;       //Wert des prescalers (Taktverminderung)  -- damit er genau 1 sekunde laueft -- 135 bei 8 MHz
  TIM1->ARR = 0xCE41;     //Auto-Reload Wert = Maximaler Zaehlerstand des Upcounters
  TIM1->RCR = 0;         //Repetition Counter deaktivieren

  /*--------------------- Interrupt Timer 1 konfigurieren ---------------------*/
  TIM1->DIER = 0x01;             //enable Interrupt bei einem UEV (Überlauf / Unterlauf)
  NVIC_SetPriority(TIM1_UP_IRQn, 2);
  NVIC_EnableIRQ(TIM1_UP_IRQn);
  clk_init();                     //ruft clk_init auf

  /*-------------------------- Timer 1 Starten -------------------------------*/
  TIM1->CR1 |= 0x0001;   //Counter-Enable bit setzen
}
/******************************************************************************/
/*                   clk_init                                                 */
/* Funktion:                                                                  */
/*   Takt auf PLL setzen, um Programm mit 72MHz realisieren zu können         */
/******************************************************************************/
void clk_init(void)
{
  FLASH->ACR = 0x12; //Set Flash latency (2 waitstates)
  RCC->CR |= RCC_CR_HSEON; //HSE on
  
  while ((RCC->CR & RCC_CR_HSERDY) == 0); // Wait for HSERDY=1 (HSE is ready)
  
  RCC->CFGR |= RCC_CFGR_PLLMULL9; // 9 mal 8 = 72 MHz (SYSCLK)
  RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6; //ADCCLK = SYSCLK/6 (APB2 PRESCALER=1)
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // PCLK1(APB1)=SYSCLK/2 (HCLK=SYSCLK)
  RCC->CFGR |= RCC_CFGR_PLLSRC; //PLL = SYSCLK,da AHB PRESCALER=1)
  RCC->CR |= RCC_CR_PLLON; //PLL on  
  //while ((RCC->CR & RCC_CR_PLLRDY) == 0); // Wait for PLLRDY=1 (PLL is ready)

  RCC->CFGR |= RCC_CFGR_SW_PLL; //PLL = Systemclock
  //while ((RCC->CFGR & RCC_CFGR_SWS_PLL) == 0);
  //Wait till SYSCLK is stabilized (depending on selected clock) */
  //while ((RCC->CFGR & RCC_CFGR_SWS) != ((RCC->CFGR<<2) & RCC_CFGR_SWS));
  // end of stm32_ClockSetup
  //RCC->BDCR |=RCC_BDCR_LSEON; //32kHz für RTC siehe AN2821 Reference Manual
  //448ff/1072 
    
  //RCC->CFGR |= 0x0241002;   //SW Register setzen fuer PLL = 2 //PLLXTPRE auf 0 setzen -- deaktivert divide by 2 // PLLMUL auf 9 setzen -- multipliziert HSE mit 9
  RCC->AHBENR |= 0x0000;    //AHB Prescaler setzen -- not divided
  RCC->APB1ENR |= 0x000;    // APB2 Prescaler setzen -- not divided*/
}

/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main (void) 
{
  clk_init();
  
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;                    // enable clock
  GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);     // reset PC13
  GPIOC->CRH |= (GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0); // config PC13
  
  TIM1_Config(); // Timer 1 starten: Upcounter --> loest alle 1s einen Update Interrupt
  while(1);
}

