/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN   All rights reserved. AUSTRIA   					*/ 
/*                                                                            */ 
/* File Name:   DC_Motor.c                                           				  */
/* Autor: 		  Berger Jonas                                                  */
/* Version: 	  V1.00                                                         */
/* Date:				07.05.2018        							                              */
/* Description: Drezahlbestimmung eines DC-Motors mit Uhr		                  */
/******************************************************************************/
/* History: 		V1.00  creation										                            */
/******************************************************************************/
#include "armv10_std.h"
#include "math.h"

/*-------------------------- Function Prototypes -----------------------------*/
void init_ports(void); //initialisiert uC-Ports
void TIM1_Config(void); //Timer 1 config, fuer Uhr
void TIM3_Config(void); //Timer 3 config, fuer Motoransteuerung
void TIM4_Config(void); //Timer 4 config, fuer Input Capture
void NVIC_init(char position, char priority); //NVIC initialisieren

/*------------------------------- BITBANDING ---------------------------------*/
#define GPIOB_ODR GPIOB_BASE + 3*sizeof(uint32_t) //Calc peripheral address GPIOB ODR
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4))) //Formel
#define MANTRIEB  *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,15))) //MANTRIEB/PB15

/* ------------------------------- Defines -----------------------------------*/
#define TIM_CounterMode_Up                 ((uint16_t)0x0000) //TIM_Counter_Mode
#define TIM_ICSelection_DirectTI           ((uint16_t)0x0001) //TIM_Input_Capture_Selection
#define TIM_ICPolarity_Rising              ((uint16_t)0x0000) //TIM_Input_Capture_Polarity
#define TIM_IT_Update                      ((uint16_t)0x0001) //TIM_interrupt_source
#define TIM_IT_CC4                         ((uint16_t)0x0010) //TIM_interrupt_source

/*---------------------------- Static Variables ------------------------------*/
int sekunden=0,minuten=0,stunden=0,zehntel=0; //Zeitvariablen

double IC4ReadValue1=0,IC4ReadValue2=0; //Input Capture Variablen
int CaptureNumber=0; //Unterscheidung 1. oder 2. Input Capture
int Capture=0; //Capture computation

int pwm_soll=5; //z.B.: 5 ... 50% High-Phase
int pwm_phase=0;
int ausw=0; //PWM: 0 ... 50:50; 1 ... einstellbar

/******************************************************************************/
/*           Interrupt Service Routine  Timer 1 (General Purpose Timer)       */
/******************************************************************************/
void TIM1_UP_IRQHandler(void)	//Timer 1, loest alle 1ms aus
{
	TIM1->SR&=~0x01; //Interrupt pending bit loeschen (Verhinderung eines nochmaligen Interrupt-ausloesens)
	if(zehntel==10)
	{
		sekunden++;
		zehntel=1;
	}
	else 
	{
		zehntel++;
	}
	if((sekunden)==60)
	{
		minuten++;
		sekunden=0;
	}
	if(minuten==60)
	{
		stunden++;
		minuten=0;
	}
	if(stunden==24)
	{
		stunden=0;
	}
}

/******************************************************************************/
/*           Interrupt Service Routine  Timer 3 (General Purpose Timer)       */
/******************************************************************************/
void TIM3_IRQHandler(void) //loest alle 5ms aus
{
	TIM3->SR&=~(0x01); //Interrupt pending bit loeschen (Verhinderung eines nochmaligen Interrupt-auslösens)
	if(ausw==0)
	{
		MANTRIEB=~MANTRIEB;
	}
	else
	{
		if(pwm_soll==pwm_phase)
		{
			MANTRIEB=0;
		}
		pwm_phase++;
		if(pwm_phase==10)
		{
			pwm_phase=0;
			MANTRIEB=1;
		}
	}
}

/******************************************************************************/
/*           Interrupt Service Routine  Timer 4 (General Purpose Timer)       */
/******************************************************************************/
void TIM4_IRQHandler(void)
{ 
  if(((TIM4->SR & TIM_IT_Update) != 0) && ((TIM4->DIER&TIM_IT_Update) != 0)) //Update Interrupt Pending ?
  {
    TIM4->SR &= ~TIM_IT_Update; //Clear TIM4 Update Interrupt pending bit
	}
  if(((TIM4->SR & TIM_IT_CC4) != 0) && ((TIM4->DIER & TIM_IT_CC4) != 0)) //Capture Interrupt von Channel 4?
	{
   TIM4->SR &= ~TIM_IT_CC4; //Clear TIM4 Capture Interrupt pending bit for Channel 4
   if(CaptureNumber == 0)
   {      
      IC4ReadValue1=TIM4->CCR4; //Get the Input Capture value for Channel 4
      CaptureNumber=1;
   }
   else if(CaptureNumber == 1)
   {      
     IC4ReadValue2=TIM4->CCR4; //Get the Input Capture value for Channel 4
     if(IC4ReadValue2 > IC4ReadValue1) //Capture computation
     {
       Capture=(IC4ReadValue2-IC4ReadValue1); 
     }
     else
     {
       Capture=((TIM4->ARR-IC4ReadValue1)+IC4ReadValue2); 
     }      
     CaptureNumber=0;
	 }
	}
}

/******************************************************************************/
/*            		Initialization Timer 1 (General Purpose Timer) 		          */
/* Funktion:                                                    		          */
/*   initialisiert Timer 1, der fuer die Uhr zustaendig ist   		            */
/******************************************************************************/
void TIM1_Config(void)
{	
	/*-------------------------- Timer 1 konfigurieren -------------------------*/
	RCC->APB2ENR |= 0x0800;	//Timer 1 Clock enable
	TIM1->SMCR = 0x0000; //Timer 1 Clock Selection: CK_INT wird verwendet
	TIM1->CR1  = 0x0000; // Auswahl des Timer Modus: Upcounter --> Zaehlt: von 0 bis zum Wert des Autoreload-Registers
	
	/*T_INT = 100ns, Annahme: Presc = 135 ---> Auto Reload Wert = 52801 (=0xCE41) --> 1s Update Event*/
	TIM1->PSC = 150; //Wert des prescalers (Taktverminderung)  -- damit er genau 1 sekunde laueft -- 135 bei 8 MHz
	TIM1->ARR = 0xCE41; //Auto-Reload Wert = Maximaler Zaehlerstand des Upcounters
	TIM1->RCR = 0; //Repetition Counter deaktivieren
	
	/*--------------------- Interrupt Timer 1 konfigurieren --------------------*/
	TIM1->DIER = 0x01; //enable Interrupt bei einem UEV (UEberlauf/Unterlauf)
	NVIC_init(TIM1_UP_IRQn,3); //Enable Timer 1 Update Interrupt, Priority 3
	
	/*---------------------------- enable Timer 1 ------------------------------*/
	TIM1->CR1 |= TIM_CR1_CEN; //Enable the Timer 1
}

/******************************************************************************/
/*            		Initialization Timer 3 (General Purpose Timer) 		          */
/* Funktion:                                                    		          */
/*   initialisiert Timer 3, der fuer die Motoransteuerung zustaendig ist      */
/******************************************************************************/
void TIM3_Config(void)
{	
	/*------------------------- Timer 3 konfigurieren --------------------------*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	//Timer 3 Clock enable
	TIM3->SMCR = 0x0000; //Timer 3 Clock Selection: CLK_INT wird verwendet
	TIM3->CR1  = 0x0000; // Auswahl des Timer Modus: Upcounter --> Zaehlt: von 0 bis zum Wert des Autoreload-Registers
	
	TIM3->PSC = 5; //Wert des Prescalers (Taktverminderung); 5ms
	TIM3->ARR = 0xCE41; //Auto-Reload Wert = Maximaler Zaehlerstand des Upcounters
	TIM3->RCR = 0; //Repetition Counter deaktivieren

	/*--------------------- Interrupt Timer 3 konfigurieren --------------------*/
	TIM3->DIER = 0x01; //enable Interrupt bei einem UEV (UEberlauf / Unterlauf)
	NVIC_init(TIM3_IRQn,2); //Enable Timer 3 Update Interrupt, Priority 2
	
	/*---------------------------- enable Timer 3 ------------------------------*/
	TIM3->CR1 |= TIM_CR1_CEN; //Enable the Timer 3
}

/******************************************************************************/
/*            		Initialization Timer 4 (General Purpose Timer) 		          */
/* Funktion:                                                    		          */
/*   initialisiert Timer 4, der fuer Input Capture am PB9 zustaendig ist      */
/******************************************************************************/
void TIM4_Config(void)
{	
	/*---------------------- Configure Timer 4 CR1 - Register ------------------*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //Timer 4 Clock enable   
  TIM4->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS); //CMS=00: Edge-aligned mode. DIR= 0 --> Upcounter
  TIM4->CR1 |= TIM_CounterMode_Up;
	
	TIM4->SMCR = 0x0000; //Slave Mode Selection: SMS 000: Counter is directly clocked by CLK_INT
	
	TIM4->PSC = 7936; //Value Prescaler (reduce system clock rate to 1KHz
	TIM4->ARR = 1000;	//Auto-Reload Value 

	/*---------------------- Configure Channel CH4(PB9) as IC ------------------*/
  TIM4->CCMR2 &= (~TIM_CCMR2_CC4S) & (~TIM_CCMR2_IC4F); //sampling of TI4 (IC4) at full speed, no filter
  TIM4->CCMR2 |= TIM_ICSelection_DirectTI <<8; //CC4S=01--> 01:Channel 4 is connected to PB9
	TIM4->CCMR2 &= ~TIM_CCMR2_IC4PSC; //IC4PSC= 00: no Input Capture 4 prescaler
	
	/*--------------- Configure Capture Compare Enable register ----------------*/
  TIM4->CCER &= ~TIM_CCER_CC4E; //Disable the Channel 4: Reset the CC4E Bit
  TIM4->CCER = TIM4->CCER;
  TIM4->CCER &= ~TIM_CCER_CC4P; //Set Polarity input capture: non-inverted: capture is done on a rising edge of IC4
  TIM4->CCER |= (TIM_ICPolarity_Rising << 4) | TIM_CCER_CC4E; //Enable Input Capture Channel 4
  TIM4->CCER = TIM4->CCER; //Write to TIM4 CCER registers
	
	/*--------------------- Enable Interrupts for Timer 4 ----------------------*/
  TIM4->DIER  |= TIM_IT_Update; //Enable Update Interrupt
  TIM4->DIER  |= TIM_IT_CC4; //Enable the CC4 Interrupt Request
  NVIC_init(TIM4_IRQn,1); //Enable Timer 4 Interrupt
	
	/*---------------------------- enable Timer 4 ------------------------------*/
	TIM4->CR1 |= TIM_CR1_CEN; //Enable the Timer 4
}

/******************************************************************************/
/*                   NVIC_init(char position, char priority)    			 			  */ 
/* Funktion:                                                                  */    
/*   UEbernimmt die vollstaendige Initialisierung eines Interrupts im Nested  */
/*   Vectored Interrupt Controller (Prioritaet setzen, Ausloesen verhindern,  */
/*   Interrupt enablen)                                                       */
/* UEbergabeparameter: "position" = 0-67 (Nummer des Interrupts)              */
/*                     "priority" : 0-15 (Prioritaet des Interrupts)		   	  */
/******************************************************************************/
void NVIC_init(char position, char priority) 
{	
	NVIC->IP[position]=(priority<<4);	//Setzen der Interrupt Prioritaet
	NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F)); //Verhindert, dass der Interrupt ausloest sobald er enabled wird 
	NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F)); //Enable interrupt
} 

/******************************************************************************/
/*                   init_ports()                                      			  */ 
/* Funktion:                                                                  */    
/*   initialisiert uC-Ports			                                              */
/******************************************************************************/
void init_ports(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //enable clock for GPIOB (APB2 Peripheral clock enable register)
	
	GPIOB->CRH &= 0x00000000; //PB15-PB8 Konfigurationsbits loeschen
	GPIOB->CRH |= 0x30000040; //PB15 auf GPOPP fuer Motorantrieb; PB9 auf IF fuer Input Capture
}

/******************************************************************************/
/*                              MAIN function                                 */
/******************************************************************************/
int main(void) 
{
	set_clock_72MHz();
	
	uart_init(9600);
	uart_clear();
	uart_put_string("DC-Motor V01\nDrezahlbestimmung eines DC-Motors mit Uhr\n");
	
	char buffer[50]; //Variable fuer LCD-Display
	
	int lcd_sekunden=0,lcd_minuten=0,lcd_stunden=0,lcd_zehntel=0; /*Variablen fuer ...*/
	char einh[10],ein[10],zeh[10],hour[10],min[10],eimin[10],sek[10]; /*... Uhrenanzeige*/
	
	double diff=0,um=0; //Variablen fuer Drezahlbestimmung
	
	
	lcd_init(); //LCD Anzeige initialisieren
	init_ports();  //initialisiert uC-Ports
	TIM1_Config(); //Timer 1 config, fuer Uhr
	TIM3_Config(); //Timer 3 config, fuer Motoransteuerung
	TIM4_Config(); //Timer 4 config, fuer Input Capture
	
	do
	{
		wait_ms(100);
		lcd_clear();
		
		/*------------------------- Uhr-Anzeige Anfang ---------------------------*/
		if (lcd_zehntel != zehntel) //1 Zehntel vergangen
		{
			lcd_zehntel = zehntel;
		}
		if (lcd_sekunden != sekunden) //1 Sekunde vergangen ?
	 	{ 
	   	lcd_sekunden = sekunden;
		}
		if (lcd_minuten != minuten) //1 Minute vergangen ?
		{
			lcd_minuten = minuten;
  	} 
		if (lcd_stunden != stunden) //1 Stunde vergangen ?
		{
				lcd_stunden = stunden;
		}
		lcd_set_cursor(0,0); // Cursor auf Ursprung
		//Stundenanzeige
		int einer = lcd_stunden/10;
		sprintf(&einh[1],"%d", einer);
    lcd_put_string(&einh[1]);  
		//Einerstundenanzeige
		int hou = lcd_stunden%10;
		sprintf(&hour[1],"%d:", hou);
    lcd_put_string(&hour[1]); 
		//Minutenanzeige Zehnerstelle
		int einermin = lcd_minuten/10;
		sprintf(&eimin[1],"%d", einermin);
		lcd_put_string(&eimin[1]);
		//Minutenanzeige Einerstelle
		int mins = lcd_minuten%10;
		sprintf(&min[1],"%d:", mins);
		lcd_put_string(&min[1]);
		//Sekundenanzeige - 
		sprintf(&sek[1],"%d", lcd_sekunden/10);
		lcd_put_string(&sek[1]);
		// - geteilt in einer und zehner stellen
		int einersek = lcd_sekunden%10;
		sprintf(&zeh[1],"%d:", einersek);
		lcd_put_string(&zeh[1]);
		//Anzeige der zehntel 
		int zehntel = lcd_zehntel%10;
		sprintf(&ein[1],"%d", zehntel);
		lcd_put_string(&ein[1]);
		/*------------------------- Uhr-Anzeige Ende -----------------------------*/
		
		/*--------------------- Drehzahl Anzeige Anfang --------------------------*/
		//Formeln fuer die Drezahlberechnung
		diff=sqrt((pow(IC4ReadValue2-IC4ReadValue1,2))); //Quadrieren um negativen Wert zu verhindern
		um=60000/diff; //Umdrehungen pro min berechnen
		
		lcd_set_cursor(1,0);
		sprintf (&buffer[0], "n=%.0lf Umd./min",um); //Drezahl Umdr./min
		lcd_put_string(&buffer[0]);
		/*--------------------- Drehzahl Anzeige Ende ----------------------------*/
	}
	while(1);
}
