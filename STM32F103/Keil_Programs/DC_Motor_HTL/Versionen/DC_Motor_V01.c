/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   LCD_EA_DOGM204_A.c                                            */
/* Autor: 		  Berger Jonas                                                  */
/* Version: 	  V1.00                                                         */
/* Date: 		    18.02.2018                                                    */
/* Description: Testprogramm fuer EA DOGM204-A LCD Display                    */
/******************************************************************************/
/* History: 	V1.00  creation										                              */
/******************************************************************************/
#include "stm32f10x.h"
#include "ARMV10_STD.h"

/*______________________________ BITBANDING __________________________________*/
#define GPIOC_ODR GPIOC_BASE + 3*sizeof(uint32_t) //Calc peripheral address GPIOC ODR
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))  
#define PC13  *((volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,13))) //PC13

/*______________________________ Prototypen __________________________________*/
void init_ports(void);
void Delay (int ms);

/*______________________________ Unterprogramme ______________________________*/
void init_ports(void) 
{
	int temp;

	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;       // enable clock for GPIOB	(APB2 Peripheral clock enable register)

	temp = GPIOC->CRH;
	temp &= 0x00000000;	// PB15-PB8 Konfigurationsbits löschen	
	temp |= 0x00300000;	// PB15-PB8 als Push Pull Output definieren
	GPIOC->CRH = temp;   
} 

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
	//init_ports();
	uart_init(9600);
	//int schalter1=0;
	
	uart_put_string("\n\nWillkommen");
	/*
	if(schalter1==0)
	{
		
	}
	else
	{
		schalter1=1;
	}
	*/
	/*while(1)
	{
		PC13=1;
		Delay(20);
		PC13=0;
		Delay(1);
	}*/
}
