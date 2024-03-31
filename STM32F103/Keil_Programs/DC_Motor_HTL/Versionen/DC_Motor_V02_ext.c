/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   DC_Motor.c                                            */
/* Autor: 		  Berger Jonas                                                  */
/* Version: 	  V1.00                                                         */
/* Date: 		    18.02.2018                                                    */
/* Description: Testprogramm fuer EA DOGM204-A LCD Display                    */
/******************************************************************************/
/* History: 	V1.00  creation										                              */
/******************************************************************************/
#include "ARMV10_STD.h"

/*------------------------------ Function Prototypes -------------------------*/
void NVIC_init(char position, char priority);
void EXTI9_5_IRQHandler(void);
void EXTI9_Config(void);
void init_ports(void);
void Delay (int ms);

/*------------------------------ Static Variables-------------------------*/
static int mpulsanz=0;


/*______________________________ BITBANDING __________________________________*/
#define GPIOC_ODR GPIOC_BASE + 3*sizeof(uint32_t) //Calc peripheral address GPIOC ODR
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))   //Formel
#define MANTRIEB  *((volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,13))) //MANTRIEB/PC13

/*______________________________ Unterprogramme ______________________________*/
/******************************************************************************/
/*           External Interrupt Service Routine                               */
/******************************************************************************/
void EXTI9_5_IRQHandler(void)//ISR
{
	EXTI->PR |= (0x01 << 9); //Pending bit EXTI9 ruecksetzen (Sonst wird die ISR immer wiederholt)
	
	if (mpulsanz==20000)
	{
		mpulsanz=0;
	}
	else
	{
		mpulsanz++;
	}
}

/******************************************************************************/
/*                                   EXTI9_config                             */ 
/* Leitung PB9 wird mit EXTI9 verbunden, Interupt von Motor, Priorität 3      */
/******************************************************************************/
void EXTI9_Config()	
{	
	NVIC_init(EXTI9_5_IRQn,3);		//NVIC fuer initialisieren EXTI Line9 (Position 7, Priority 3)
	
  RCC->APB2ENR |= 0x0001;		     //AFIOEN  - Clock enable
	AFIO->EXTICR[2] &= 0x0010;     //Interrupt-Line EXTI9 mit Portpin PB13 verbinden
	//EXTI->SWIER |= 0x0200;
	EXTI->FTSR |= (0x01 << 9);	   //Falling Edge Trigger für EXTI9 Aktivieren
  EXTI->RTSR &=~(0x01 << 9);	   //Rising Edge Trigger für EXTI9 Deaktivieren

	EXTI->PR |= (0x01 << 9);	     //EXTI9_clear_pending: Das Auslösen auf vergangene Vorgänge nach	dem enablen verhindern
	EXTI->IMR |= (0x01 << 9);      //Enable Interrupt EXTI9-Line. Kann durch den NVIC jedoch noch maskiert werden
}

/******************************************************************************/
/*                   NVIC_init(char position, char priority)    			        */ 
/* Funktion:                                                                  */    
/*   UEbernimmt die vollständige initialisierung eines Interrupts  im Nested  */
/*   vectored Interrupt controller (Prioritaet setzen, Ausloesen verhindern,  */
/*   Interrupt enablen)                                                       */
/*   UEbergabeparameter: "position" = 0-67 (Nummer des Interrupts)            */
/*                    "priority": 0-15 (Prioritaet des Interrupts)		        */
/******************************************************************************/
static void NVIC_init(char position, char priority) 
{	
	NVIC->IP[position]=(priority<<4);	//Interrupt priority register: Setzen der Interrupt Prioritaet
	NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Clear Pendig Register: Verhindert, dass der Interrupt ausloest sobald er enabled wird 
	NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Set Enable Register: Enable interrupt
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
	temp |= 0x00300000;
	GPIOC->CRH = temp;
	
	//MPULS/PB9
	temp = GPIOB->CRH;
	temp &= 0x00000000;	// PB15-PB8 Konfigurationsbits loeschen	
	temp |= 0x00000080; // PB9 auf IF
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
/*                                MAIN function                               */
/******************************************************************************/
int main(void)
{
	init_ports();
	
  lcd_init ();	   		// Initialisieren der LCD Anzeige
	lcd_clear();		 		// LCD-Anzeigeninhalt loeschen
	
	EXTI9_Config();	    // Konfigurieren des Externen Interrupts für Leitung PB9
	
	char buffer[10];
	
	while(1)
	{
		lcd_set_cursor(0,0);
		lcd_put_string("Anzahl der Imp.:");
		lcd_set_cursor(1,0);
		sprintf(&buffer[0],"%d",mpulsanz);
		lcd_put_string(&buffer[0]);
		
		MANTRIEB=1;
		Delay(5);
		MANTRIEB=0;
		Delay(5);
	}
}
