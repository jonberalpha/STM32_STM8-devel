/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   Timer_Capture.c                                               */
/* Autor: 		Berger Jonas                                                    */
/* Version: 	V1.00                                                           */
/* Date: 		29/04/2018                                                        */
/* Description: Capture Eingang   															              */
/******************************************************************************/
/* History: 	V1.00  creation										          */
/******************************************************************************/
#include "armv10_std.h"
#include "math.h"

/*------------------------------ Function Prototypes -------------------------*/
static void TIM4_Config(void);
static void NVIC_init(char position, char priority);
static void init_ports(void);

/*------------------------------ Static Variables-------------------------*/
/*__IO uint16_t*/double IC4ReadValue1 = 0, IC4ReadValue2 = 0;
__IO uint16_t CaptureNumber = 0;
__IO uint32_t Capture = 0;


/*------------------------------- BITBANDING ---------------------------------*/
#define GPIOC_ODR GPIOC_BASE + 3*sizeof(uint32_t) //Calc peripheral address GPIOC ODR
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))   //Formel
#define MANTRIEB  *((volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,13))) //MANTRIEB/PC13


/* ------------------------------------------- TIM_Counter_Mode -------------------------------*/
#define TIM_CounterMode_Up                 ((uint16_t)0x0000)

/*******************  Bit definition for TIM_CR1 register  ********************/
#define  TIM_CR1_CEN                       ((uint16_t)0x0001)  /* Counter enable */
#define  TIM_CR1_DIR                       ((uint16_t)0x0010)  /* Direction */
#define  TIM_CR1_CMS                       ((uint16_t)0x0060)  /* CMS[1:0] bits (Center-aligned mode selection) */

/* ----------------------------------------------TIM_Input_Capture_Selection -----------------------*/
#define TIM_ICSelection_DirectTI           ((uint16_t)0x0001) /* TIM Input 1, 2, 3 or 4 is selected to be connected to IC1, IC2, IC3 or IC4, respectively */
		
/* ------------------------------------------  Bit definition for TIM_CCER register  -----------------------------*/
#define  TIM_CCER_CC4E                       ((uint16_t)0x1000)            /*!< Capture/Compare 4 output enable */
#define  TIM_CCER_CC4P                       ((uint16_t)0x2000)            /*!< Capture/Compare 4 output Polarity */
//#define  TIM_CCER_CC4NP                      ((uint16_t)0x8000)            /*!< Capture/Compare 4 Complementary output Polarity */

/* ------------------------------------------------- TIM_Input_Capture_Polarity -----------------------------*/
#define  TIM_ICPolarity_Rising             ((uint16_t)0x0000)

/* ------------------------------ TIM_interrupt_sources -------------------------*/
#define TIM_IT_Update                      ((uint16_t)0x0001)
#define TIM_IT_CC4                         ((uint16_t)0x0010)

/******************************************************************************/
/*                   NVIC_init(char position, char priority)    			  */ 
/* Funktion:                                                                  */    
/*   UEbernimmt die vollstaendige initialisierung eines Interrupts  im Nested   */
/*   vectored Interrupt controller (Prioritaet setzen, Ausloesen verhindern,   */
/*   Interrupt enablen)                                                       */
/* UEbergabeparameter: "position" = 0-67 (Nummer des Interrupts)               */
/*                    "priority": 0-15 (Prioritaet des Interrupts)		      */
/******************************************************************************/
static void NVIC_init(char position, char priority) 
{	
	NVIC->IP[position]=(priority<<4);	//Interrupt priority register: Setzen der Interrupt Prioritaet
	NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Clear Pendig Register: Verhindert, dass der Interrupt ausloest sobald er enabled wird 
	NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Set Enable Register: Enable interrupt
} 

/******************************************************************************/
/*                          Timer 4 Interrupt Service Routine                 */
/******************************************************************************/
void TIM4_IRQHandler(void)
{ 
  if (((TIM4->SR & TIM_IT_Update) != 0) && ((TIM4->DIER & TIM_IT_Update) != 0))		// Update Interrupt Pending ?
  {
    TIM4->SR &= ~TIM_IT_Update; /* Clear TIM4 Update Interrupt pending bit */
	  //LED7=~LED7;
  }
  if (((TIM4->SR & TIM_IT_CC4) != 0) && ((TIM4->DIER & TIM_IT_CC4) != 0))		// Capture Interrupt von Channel 4?
	{
   TIM4->SR &= ~TIM_IT_CC4; /* Clear TIM4 Capture Interrupt pending bit for Channel 4*/
   if(CaptureNumber == 0)
   {      
      IC4ReadValue1 = TIM4->CCR4;/* Get the Input Capture value for Channel 4*/
      CaptureNumber = 1;
   }
   else if(CaptureNumber == 1)
   {      
     IC4ReadValue2 = TIM4->CCR4; /* Get the Input Capture value for Channel 4*/
     if (IC4ReadValue2 > IC4ReadValue1) /* Capture computation */
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
static void TIM4_Config(void)
{	
	uint16_t tmp = 0;

	/*---------------------- Configure Timer 4 CR1 - Register -----------------------*/
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
  tmp &= (~TIM_CCMR2_CC4S) & (~TIM_CCMR2_IC4F); /* sampling of TI2 (Input Capture channel 4) at full speed, not filter */

  tmp |= TIM_ICSelection_DirectTI <<8;				 /* CC4S=01--> 01: CC4 channel  Timer 2 is configured as input capture, */
	                                         /* IC2 is mapped on TI2.  Channel 4 is connected to PA1 */
	tmp &= ~TIM_CCMR2_IC4PSC; /* IC2PSC= 00: no Input Capture 1 prescaler */ 
	                          /* capture is done each time an edge is detected on the capture input */  
	TIM4->CCMR2 = tmp ; /* Write to TIM4 CCMR2 registers */
	
	/* -------------- Configure Capture Compare Enable register for Input Capture Channel 4 ---------------*/
  TIM4->CCER &= ~TIM_CCER_CC4E;   /* Disable the Channel 4: Reset the CC4E Bit */
  tmp = TIM4->CCER;
  tmp &= ~TIM_CCER_CC4P; /* Set Polarity input capture: non-inverted: capture is done on a rising edge of IC4 */
  tmp |= (TIM_ICPolarity_Rising << 4) | TIM_CCER_CC4E;  /* Enable Input Capture Channel 4 */
  TIM4->CCER = tmp;      /* Write to TIM4 CCER registers */
	
	/* -------------- Enable Interrupts for Timer 4 -------------------------*/	
  TIM4->DIER  |= TIM_IT_Update;   /* Enable Update Interrupt */
  TIM4->DIER  |= TIM_IT_CC4;     /* Enable the CC4 Interrupt Request */
  NVIC_init(TIM4_IRQn,1);        /* Enable Timer 4 Interrupt  */
 
}

/******************************************************************************/
/*                   init_ports()                                      			  */ 
/* Funktion:                                                                  */    
/*   initialisiert die GPIO Ports                                             */
/******************************************************************************/
void init_ports(void) 
{
	int temp;

	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;       // enable clock for GPIOC	(APB2 Peripheral clock enable register)
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;       // enable clock for GPIOB	(APB2 Peripheral clock enable register)
	
	//MANTRIEB/PC13
	temp = GPIOC->CRH;
	//Konfigurationsbits nicht loeschen, sonst TIM4 geht nicht
	temp |= 0x00300000; // PC13 auf GPOPP
	GPIOC->CRH = temp;
	
	//MPULS/PB9
	temp = GPIOB->CRH;
	temp &= 0x00000000;	// PB15-PB8 Konfigurationsbits loeschen	
	temp |= 0x00000040; // PB9 auf IF
	GPIOB->CRH = temp;
}

/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main (void) 
{
	char buffer[50];

	lcd_init();             // LCD Anzeige initialisieren
	init_leds_switches();	  // LED-Schalter Platine initialisieren (Schalter PA0-PA7, LED PB8-PB15
	init_ports();
	TIM4_Config();  				// Configure Timer 4
	TIM4->CR1 |= TIM_CR1_CEN;     //Enable the TIM4 Counter 

	do
	{
		wait_ms(100);
		lcd_clear();   
		lcd_set_cursor(0,0);	 
		sprintf (&buffer[0], "CNT=%d",TIM4->CNT); 
		lcd_put_string(&buffer[0]);		
		
		//Die zwei Zaehlerstaende
		lcd_set_cursor(1,0);
		sprintf (&buffer[0], "%.0lf:%.0lf",IC4ReadValue1,IC4ReadValue2); 
		lcd_put_string(&buffer[0]);
		
		//Differenz
		lcd_set_cursor(1,10);
		sprintf (&buffer[0], "%.0lf",sqrt((pow(IC4ReadValue2-IC4ReadValue1,2))));
		lcd_put_string(&buffer[0]);	
	}
	while(1);

}
