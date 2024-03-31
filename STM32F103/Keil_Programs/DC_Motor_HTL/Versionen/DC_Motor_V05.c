/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN   All rights reserved. AUSTRIA   					*/ 
/*                                                                            */ 
/* File Name:   DC_Motor.c                                           				  */
/* Autor: 		  Berger Jonas                                                  */
/* Version: 	  V1.00                                                         */
/* Date:(last change) 		    25.04.2018                                      */
/* Description: UHR-Anzeige und Drehzahlberechnung					                  */
/******************************************************************************/
/* History: 	V1.00  creation										                              */
/******************************************************************************/
#include "armv10_std.h"						 
#include "math.h"

/*------------------------------ Function Prototypes -------------------------*/
void clock_init(void);
void TIM4_config(void);
void NVIC_init(char position, char priority);
void init_ports(void);
void Delay (int ms);

/* ----------------------------- TIM_Counter_Mode ----------------------------*/
#define TIM_CounterMode_Up	((uint16_t)0x0000)

/* --------------------------- TIM_interrupt_sources -------------------------*/
#define TIM_IT_Update				((uint16_t)0x0001)
#define TIM_IT_CC4					((uint16_t)0x0010)

/*------------------------------- BITBANDING ---------------------------------*/
#define GPIOC_ODR GPIOC_BASE + 3*sizeof(uint32_t) //Calc peripheral address GPIOC ODR
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))   //Formel
#define MANTRIEB  *((volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,13))) //MANTRIEB/PC13

/*------------------------------ Static Variables-------------------------*/
int IC4ReadValue1 = 0, IC4ReadValue2 = 0;
int CaptureNumber = 0;
long Capture = 0;

/******************************************************************************/
/*                   init_ports()                                      			  */ 
/* Funktion:                                                                  */    
/*   initialisiert die GPIO Ports                                             */
/******************************************************************************/
void clock_init(void)
{
	RCC->CFGR |= 0x0240002;		//SW Register setzten fuer PLL=2
	RCC->AHBENR |= 0x0000;
	RCC->APB1ENR |= 0x0000;
}

/******************************************************************************/
/*                   init_ports()                                      			  */ 
/* Funktion:                                                                  */    
/*   initialisiert die GPIO Ports                                             */
/******************************************************************************/
void init_ports(void) 
{
	int temp;

	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;       // enable clock for GPIOB	(APB2 Peripheral clock enable register)
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;       // enable clock for GPIOB	(APB2 Peripheral clock enable register)
	
	//MANTRIEB/PC13
	temp = GPIOC->CRH;
	temp &= 0x00000000;	// PC15-PC8 Konfigurationsbits loeschen	
	temp |= 0x00300000; // PC13 auf GPOPP
	GPIOC->CRH = temp;
	
	//MPULS/PB9
	temp = GPIOB->CRH;
	temp &= 0x00000000;	// PB15-PB8 Konfigurationsbits loeschen	
	temp |= 0x00000080; // PB9 auf IPU
	GPIOB->CRH = temp;
	
	temp = GPIOB->ODR;
	temp &= 0x0000;
	temp |= 0x0200;
	GPIOB->ODR = temp;
}

/******************************************************************************/
/*                   Delay(ms)                                       			    */ 
/* Funktion:                                                                  */    
/*   wartet die angegebene Zeit in ms ab                                      */
/******************************************************************************/
void Delay (int ms)
{
	int i,j;

	for(i = 0; i < ms; i++){
	for(j = 0; j < (8000); j++){
	}}
}


/******************************************************************************/
/*                   NVIC_init(char position, char priority)    			  */ 
/* Funktion:                                                                  */    
/*   Übernimmt die vollständige initialisierung eines Interrupts  im Nested   */
/*   vectored Interrupt controller (Priorität setzen, Auslösen verhindern,    */
/*   Interrupt enablen)                                                       */
/* Übergabeparameter: "position" = 0-67 (Nummer des Interrupts)               */
/*                    "priority": 0-15 (Priorität des Interrupts)		      */
/******************************************************************************/
void NVIC_init(char position, char priority) 
{	
	NVIC->IP[position]=(priority<<4);	//Interrupt priority register: Setzen der Interrupt Priorität
	NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F)); //Interrupt Clear Pendig Register: Verhindert, dass der Interrupt auslöst sobald er enabled wird 
	NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F)); //Interrupt Set Enable Register: Enable interrupt
} 

/******************************************************************************/
/*                          Timer 4 Interrupt Service Routine                 */
/******************************************************************************/
void TIM4_IRQHandler(void)
{ 
  if(((TIM4->SR & TIM_IT_CC4) != 0) && ((TIM4->DIER & TIM_IT_CC4) != 0))		// Capture Interrupt von Channel 4?
	{
   TIM4->SR &= ~TIM_IT_CC4; // Clear TIM4 Capture Interrupt pending bit for Channel 4
   if(CaptureNumber == 0)
   {      
      IC4ReadValue1 = TIM4->CCR4; // Get the Input Capture value for Channel 4*/
      CaptureNumber = 1;
   }
   else if(CaptureNumber == 1)
   {      
     IC4ReadValue2 = TIM4->CCR4; // Get the Input Capture value for Channel 2
     if (IC4ReadValue2 > IC4ReadValue1) // Capture computation
     {
       Capture = (IC4ReadValue2 - IC4ReadValue1); 
     }
     else
     {
       Capture = ((TIM4->ARR - IC4ReadValue1) + IC4ReadValue2); 
     }      
     CaptureNumber = 0;
	 }
	}
}
																																																							
/******************************************************************************/
/*                     Initialization Timer4 (General Purpose Timer)          */
/******************************************************************************/
void TIM4_config(void)
{	
	int tmp = 0;

	/*---------------------- Configure TIM4_CR1 ---------------------------*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;    //Timer 4 Clock enable
	
  tmp = TIM4->CR1;
  tmp &= 0x0000;  // CMS=00: Edge-aligned mode. The counter counts up or down depending
  tmp |= TIM_CounterMode_Up;					  // on the direction bit. DIR= 0 --> Upcounter
  TIM4->CR1 = tmp;

	TIM4->SMCR |= 0x0000; 		//Slave Mode Selection: SMS 000: Counter is directly clocked by CLK_INT

	TIM4->PSC |= 0x0F;		    //Value Prescaler (reduce system clock rate to 1KHz
	TIM4->ARR |= 0xFFFF;		    //Auto-Reload Value, max zaehlerstand

	/* -------------- Configure CCER Channel 4 ----------------------------*/
  TIM4->CCER &= ~TIM_CCER_CC4E;   // Disable the Channel 4: Reset the CC4E Bit
	TIM4->CCER &= ~TIM_CCER_CC4P; 	// Set Polarity input capture: non-inverted: capture is done on a rising edge of IC4
  TIM4->CCER |= TIM_CCER_CC4E;  // Enable Input Capture Channel 4
	
	/* -------------- Configure Channel CH4(PB9) TIM4_CCMR2 ---------------*/
  tmp = TIM4->CCMR2;
  tmp |= 0x0100; //CH4 ist Input Capture fuer TIM4, no perescaler
	tmp &= 0x0FFF; //Konfiguriere IC4 filer für TIM4, no filter
	TIM4->CCMR2 = tmp ;
	
	/* -------------- Enable Interrupts for Timer 4 -----------------------*/	
  TIM4->DIER  |= TIM_IT_Update;   // Enable Update Interrupt
  TIM4->DIER  |= TIM_IT_CC4;      // Enable the CC4 Interrupt Request
  NVIC_init(TIM4_IRQn,1);         // Enable Timer 4 Interrupt
}

/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main (void) 
{
	//clock_init();
	init_ports();
	lcd_init();             		// LCD Anzeige initialisieren
	lcd_clear(); 								// LCD-Anzeigeninhalt loeschen
	TIM4_config();  						// Configure Timer 4
	TIM4->CR1 |= TIM_CR1_CEN;   // Enable the TIM4 Counter
	
	char buffer[50];
	
	while(1)
	{
		lcd_set_cursor(0,0);
		sprintf(&buffer[0], "CNT=%d",TIM4->CNT);
		lcd_put_string(&buffer[0]);

		lcd_set_cursor(1,0);
		sprintf (&buffer[0], "%d:%d",IC4ReadValue1,IC4ReadValue2);
		lcd_put_string(&buffer[0]);
		
		MANTRIEB=1;
		Delay(5);
		MANTRIEB=0;
		Delay(5);
	}
}
