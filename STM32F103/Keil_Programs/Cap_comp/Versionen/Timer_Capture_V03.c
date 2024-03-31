/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   Timer_Capture.c                                               */
/* Autor: 		Josef Reisinger                                                 */
/* Version: 	V1.00                                                           */
/* Date: 		16/01/2016                                                        */
/* Description: Demoprogramm für Capture Eingang an GP Timer 2                */
/******************************************************************************/
/* History: 	V1.00  creation										          */
/******************************************************************************/
#include "armv10_std.h"						 

/*------------------------------ Function Prototypes -------------------------*/
static void TIM4_Config(void);
static void NVIC_init(char position, char priority);
void init_ports(void);

/*------------------------------ Static Variables-------------------------*/
__IO uint16_t IC2ReadValue1 = 0, IC2ReadValue2 = 0;
__IO uint16_t CaptureNumber = 0;
__IO uint32_t Capture = 0;


/*------------------------------- BITBANDING ---------------------------------*/
#define GPIOC_ODR GPIOC_BASE + 3*sizeof(uint32_t) //Calc peripheral address GPIOC ODR
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))   //Formel
#define MANTRIEB  *((volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,13))) //MANTRIEB/PC13


/* ------------------------------------------- TIM_Counter_Mode -------------------------------*/
#define TIM_CounterMode_Up                 ((uint16_t)0x0000)
#define TIM_CounterMode_Down               ((uint16_t)0x0010)
#define TIM_CounterMode_CenterAligned1     ((uint16_t)0x0020)
#define TIM_CounterMode_CenterAligned2     ((uint16_t)0x0040)
#define TIM_CounterMode_CenterAligned3     ((uint16_t)0x0060)

/*******************  Bit definition for TIM_CR1 register  ********************/
#define  TIM_CR1_CEN                       ((uint16_t)0x0001)  /* Counter enable */
#define  TIM_CR1_DIR                       ((uint16_t)0x0010)  /* Direction */
#define  TIM_CR1_CMS                       ((uint16_t)0x0060)  /* CMS[1:0] bits (Center-aligned mode selection) */

/*-----------------------------------------------  Bit definition for TIM_CCMR1 register  --------------------------*/
#define  TIM_CCMR1_CC1S                      ((uint16_t)0x0003)            /*!< CC1S[1:0] bits (Capture/Compare 1 Selection) */
#define  TIM_CCMR1_CC1S_0                    ((uint16_t)0x0001)            /*!< Bit 0 */
#define  TIM_CCMR1_CC1S_1                    ((uint16_t)0x0002)            /*!< Bit 1 */

#define  TIM_CCMR1_OC1FE                     ((uint16_t)0x0004)            /*!< Output Compare 1 Fast enable */
#define  TIM_CCMR1_OC1PE                     ((uint16_t)0x0008)            /*!< Output Compare 1 Preload enable */

#define  TIM_CCMR1_OC1M                      ((uint16_t)0x0070)            /*!< OC1M[2:0] bits (Output Compare 1 Mode) */
#define  TIM_CCMR1_OC1M_0                    ((uint16_t)0x0010)            /*!< Bit 0 */
#define  TIM_CCMR1_OC1M_1                    ((uint16_t)0x0020)            /*!< Bit 1 */
#define  TIM_CCMR1_OC1M_2                    ((uint16_t)0x0040)            /*!< Bit 2 */

#define  TIM_CCMR1_OC1CE                     ((uint16_t)0x0080)            /*!< Output Compare 1Clear Enable */

#define  TIM_CCMR1_CC2S                      ((uint16_t)0x0300)            /*!< CC2S[1:0] bits (Capture/Compare 2 Selection) */
#define  TIM_CCMR1_CC2S_0                    ((uint16_t)0x0100)            /*!< Bit 0 */
#define  TIM_CCMR1_CC2S_1                    ((uint16_t)0x0200)            /*!< Bit 1 */

#define  TIM_CCMR1_OC2FE                     ((uint16_t)0x0400)            /*!< Output Compare 2 Fast enable */
#define  TIM_CCMR1_OC2PE                     ((uint16_t)0x0800)            /*!< Output Compare 2 Preload enable */

#define  TIM_CCMR1_OC2M                      ((uint16_t)0x7000)            /*!< OC2M[2:0] bits (Output Compare 2 Mode) */
#define  TIM_CCMR1_OC2M_0                    ((uint16_t)0x1000)            /*!< Bit 0 */
#define  TIM_CCMR1_OC2M_1                    ((uint16_t)0x2000)            /*!< Bit 1 */
#define  TIM_CCMR1_OC2M_2                    ((uint16_t)0x4000)            /*!< Bit 2 */

#define  TIM_CCMR1_OC2CE                     ((uint16_t)0x8000)            /*!< Output Compare 2 Clear Enable */

#define  TIM_CCMR1_IC1PSC                    ((uint16_t)0x000C)            /*!< IC1PSC[1:0] bits (Input Capture 1 Prescaler) */
#define  TIM_CCMR1_IC1PSC_0                  ((uint16_t)0x0004)            /*!< Bit 0 */
#define  TIM_CCMR1_IC1PSC_1                  ((uint16_t)0x0008)            /*!< Bit 1 */

#define  TIM_CCMR1_IC1F                      ((uint16_t)0x00F0)            /*!< IC1F[3:0] bits (Input Capture 1 Filter) */
#define  TIM_CCMR1_IC1F_0                    ((uint16_t)0x0010)            /*!< Bit 0 */
#define  TIM_CCMR1_IC1F_1                    ((uint16_t)0x0020)            /*!< Bit 1 */
#define  TIM_CCMR1_IC1F_2                    ((uint16_t)0x0040)            /*!< Bit 2 */
#define  TIM_CCMR1_IC1F_3                    ((uint16_t)0x0080)            /*!< Bit 3 */

#define  TIM_CCMR1_IC2PSC                    ((uint16_t)0x0C00)            /*!< IC2PSC[1:0] bits (Input Capture 2 Prescaler) */
#define  TIM_CCMR1_IC2PSC_0                  ((uint16_t)0x0400)            /*!< Bit 0 */
#define  TIM_CCMR1_IC2PSC_1                  ((uint16_t)0x0800)            /*!< Bit 1 */

#define  TIM_CCMR1_IC2F                      ((uint16_t)0xF000)            /*!< IC2F[3:0] bits (Input Capture 2 Filter) */
#define  TIM_CCMR1_IC2F_0                    ((uint16_t)0x1000)            /*!< Bit 0 */
#define  TIM_CCMR1_IC2F_1                    ((uint16_t)0x2000)            /*!< Bit 1 */
#define  TIM_CCMR1_IC2F_2                    ((uint16_t)0x4000)            /*!< Bit 2 */
#define  TIM_CCMR1_IC2F_3                    ((uint16_t)0x8000)            /*!< Bit 3 */

/* ----------------------------------------------TIM_Input_Capture_Selection -----------------------*/
#define TIM_ICSelection_DirectTI           ((uint16_t)0x0001) /* TIM Input 1, 2, 3 or 4 is selected to be connected to IC1, IC2, IC3 or IC4, respectively */
#define TIM_ICSelection_IndirectTI         ((uint16_t)0x0002) /* TIM Input 1, 2, 3 or 4 is selected to be connected to IC2, IC1, IC4 or IC3, respectively. */
#define TIM_ICSelection_TRC                ((uint16_t)0x0003) /* TIM Input 1, 2, 3 or 4 is selected to be connected to TRC. */
		
/* ------------------------------------------  Bit definition for TIM_CCER register  -----------------------------*/
#define  TIM_CCER_CC1E                       ((uint16_t)0x0001)            /*!< Capture/Compare 1 output enable */
#define  TIM_CCER_CC1P                       ((uint16_t)0x0002)            /*!< Capture/Compare 1 output Polarity */
#define  TIM_CCER_CC1NE                      ((uint16_t)0x0004)            /*!< Capture/Compare 1 Complementary output enable */
#define  TIM_CCER_CC1NP                      ((uint16_t)0x0008)            /*!< Capture/Compare 1 Complementary output Polarity */
#define  TIM_CCER_CC2E                       ((uint16_t)0x0010)            /*!< Capture/Compare 2 output enable */
#define  TIM_CCER_CC2P                       ((uint16_t)0x0020)            /*!< Capture/Compare 2 output Polarity */
#define  TIM_CCER_CC2NE                      ((uint16_t)0x0040)            /*!< Capture/Compare 2 Complementary output enable */
#define  TIM_CCER_CC2NP                      ((uint16_t)0x0080)            /*!< Capture/Compare 2 Complementary output Polarity */
#define  TIM_CCER_CC3E                       ((uint16_t)0x0100)            /*!< Capture/Compare 3 output enable */
#define  TIM_CCER_CC3P                       ((uint16_t)0x0200)            /*!< Capture/Compare 3 output Polarity */
#define  TIM_CCER_CC3NE                      ((uint16_t)0x0400)            /*!< Capture/Compare 3 Complementary output enable */
#define  TIM_CCER_CC3NP                      ((uint16_t)0x0800)            /*!< Capture/Compare 3 Complementary output Polarity */
#define  TIM_CCER_CC4E                       ((uint16_t)0x1000)            /*!< Capture/Compare 4 output enable */
#define  TIM_CCER_CC4P                       ((uint16_t)0x2000)            /*!< Capture/Compare 4 output Polarity */
#define  TIM_CCER_CC4NP                      ((uint16_t)0x8000)            /*!< Capture/Compare 4 Complementary output Polarity */

/* ------------------------------------------------- TIM_Input_Capture_Polarity -----------------------------*/
#define  TIM_ICPolarity_Rising             ((uint16_t)0x0000)
#define  TIM_ICPolarity_Falling            ((uint16_t)0x0002)
#define  TIM_ICPolarity_BothEdge           ((uint16_t)0x000A)

/* ------------------------------ TIM_interrupt_sources -------------------------*/
#define TIM_IT_Update                      ((uint16_t)0x0001)
#define TIM_IT_CC1                         ((uint16_t)0x0002)
#define TIM_IT_CC2                         ((uint16_t)0x0004)
#define TIM_IT_CC3                         ((uint16_t)0x0008)
#define TIM_IT_CC4                         ((uint16_t)0x0010)
#define TIM_IT_COM                         ((uint16_t)0x0020)
#define TIM_IT_Trigger                     ((uint16_t)0x0040)
#define TIM_IT_Break                       ((uint16_t)0x0080)

/******************************************************************************/
/*                   NVIC_init(char position, char priority)    			  */ 
/* Funktion:                                                                  */    
/*   Übernimmt die vollständige initialisierung eines Interrupts  im Nested   */
/*   vectored Interrupt controller (Priorität setzen, Auslösen verhindern,    */
/*   Interrupt enablen)                                                       */
/* Übergabeparameter: "position" = 0-67 (Nummer des Interrupts)               */
/*                    "priority": 0-15 (Priorität des Interrupts)		      */
/******************************************************************************/
static void NVIC_init(char position, char priority) 
{	
	NVIC->IP[position]=(priority<<4);	//Interrupt priority register: Setzen der Interrupt Priorität
	NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Clear Pendig Register: Verhindert, dass der Interrupt auslöst sobald er enabled wird 
	NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Set Enable Register: Enable interrupt
} 

/******************************************************************************/
/*                          Timer 2 Interrupt Service Routine                 */
/******************************************************************************/
void TIM4_IRQHandler(void)
{ 
  if (((TIM4->SR & TIM_IT_Update) != 0) && ((TIM4->DIER & TIM_IT_Update) != 0))		// Update Interrupt Pending ?
  {
    TIM4->SR &= ~TIM_IT_Update; /* Clear TIM4 Update Interrupt pending bit */
	  LED7=~LED7;
  }
  if (((TIM4->SR & TIM_IT_CC2) != 0) && ((TIM4->DIER & TIM_IT_CC2) != 0))		// Capture Interrupt von Channel 2?
	{
   TIM4->SR &= ~TIM_IT_CC2; /* Clear TIM2 Capture Interrupt pending bit for Channel 2*/
   if(CaptureNumber == 0)
   {      
      IC2ReadValue1 = TIM4->CCR2;/* Get the Input Capture value for Channel 2*/
      CaptureNumber = 1;
   }
   else if(CaptureNumber == 1)
   {      
     IC2ReadValue2 = TIM4->CCR2; /* Get the Input Capture value for Channel 2*/
     if (IC2ReadValue2 > IC2ReadValue1) /* Capture computation */
     {
       Capture = (IC2ReadValue2 - IC2ReadValue1); 
     }
     else
     {
       Capture = ((TIM4->ARR - IC2ReadValue1) + IC2ReadValue2); 
     }      
     CaptureNumber = 0;
	 }
	}
}


																																																															
/******************************************************************************/
/*                     Initialization Timer2 (General Purpose Timer)          */
/******************************************************************************/
static void TIM4_Config(void)
{	
uint16_t tmp = 0;

	/*---------------------- Configure Timer 2 CR1 - Register -----------------------*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;    //Timer 4 Clock enable
  tmp = TIM4->CR1;     
  tmp &= ~(TIM_CR1_DIR | TIM_CR1_CMS);  /* CMS=00: Edge-aligned mode. The counter counts up or down depending */
  tmp |= TIM_CounterMode_Up;					 /* on the direction bit. DIR= 0 --> Upcounter */
  TIM4->CR1 = tmp;

	TIM4->SMCR = 0x0000; 	//Slave Mode Selection: SMS 000: Counter is directly clocked by CLK_INT

	TIM4->PSC = 7936;		    //Value Prescaler (reduce system clock rate to 1KHz
	TIM4->ARR = 1000;		    //Auto-Reload Value 

	/* -------------- Configure Channel CH2(PA1) as Input Capture-Channel  (Capture Compare mode Register 1) ---------------*/
  tmp = TIM4->CCMR2;
  tmp &= (~TIM_CCMR1_CC2S) & (~TIM_CCMR1_IC2F); /* sampling of TI2 (Input Capture channel 2) at full speed, not filter */

  tmp |= TIM_ICSelection_DirectTI <<8;				 /* CC2S=01--> 01: CC2 channel  Timer 2 is configured as input capture, */
	                                         /* IC2 is mapped on TI2.  Channel 2 is connected to PA1 */
	tmp &= ~TIM_CCMR1_IC2PSC; /* IC2PSC= 00: no Input Capture 1 prescaler */ 
	                          /* capture is done each time an edge is detected on the capture input */  
	TIM4->CCMR1 = tmp ; /* Write to TIM2 CCMR2 registers */
	
	/* -------------- Configure Capture Compare Enable register for Input Capture Channel 2 ---------------*/
  TIM4->CCER &= ~TIM_CCER_CC2E;   /* Disable the Channel 2: Reset the CC2E Bit */
  tmp = TIM4->CCER;
  tmp &= ~TIM_CCER_CC2P; /* Set Polarity input capture: non-inverted: capture is done on a rising edge of IC2 */
  tmp |= (TIM_ICPolarity_Rising << 4) | TIM_CCER_CC2E;  /* Enable Input Capture Channel 2 */
  TIM4->CCER = tmp;      /* Write to TIM2 CCER registers */
	
	/* -------------- Enable Interrupts for Timer 2 -------------------------*/	
  TIM4->DIER  |= TIM_IT_Update;   /* Enable Update Interrupt */
  TIM4->DIER  |= TIM_IT_CC2;     /* Enable the CC2 Interrupt Request */
  NVIC_init(TIM4_IRQn,1);        /* Enable Timer 2 Interrupt  */
 
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
/*                                MAIN function                               */
/******************************************************************************/
int main (void) 
{
char buffer[50];

	init_ports();
	lcd_init();             // LCD Anzeige initialisieren
	init_leds_switches();	  // LED-Schalter Platine initialisieren (Schalter PA0-PA7, LED PB8-PB15
	
	TIM4_Config();  // Configure Timer 2 
	/*-------------------------- Timer 2 Starten -------------------------------*/
	TIM4->CR1 |= TIM_CR1_CEN;     /* Enable the TIM2 Counter */  

do {
	
  wait_ms(100);
  lcd_clear();   
	lcd_set_cursor(0,0);	 
	sprintf (&buffer[0], "CNT=%d",TIM4->CNT); 
	lcd_put_string(&buffer[0]);		

	lcd_set_cursor(1,0);
	sprintf (&buffer[0], "%d:%d ",IC2ReadValue1,IC2ReadValue2); 
	lcd_put_string(&buffer[0]);		

}
while (1);

}
