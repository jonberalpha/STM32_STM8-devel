/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2018 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   main.c                                                        */
/* Autor: 	    Berger Jonas                                                  */
/* Version: 	  V1.00                                                         */
/* Date: 		    08.06.2019                                                    */
/* Description: 1-wire Temperatursensor auf dem Minimalsystem (PB1) ansteueren*/
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/

/*_______________________________ INCLUDES ___________________________________*/
#include <armv10_std.h>

/*________________________________ DEFINES ___________________________________*/
//Define DS18S20 Command Set
#define ConvertT        0x44
#define ReadScratchpad  0xBE
#define WriteScratchpad 0x4E
#define CopyScratchpad  0x48
#define RecallE2        0xB8
#define ReadPowerSupply 0xB4

/*______________________________ BITBANDING __________________________________*/
//Calc peripheral address GPIOB ODR:
#define GPIOB_ODR GPIOB_BASE + 3*sizeof(uint32_t)
//Calc Bit Band Adress from peripheral address: a = peripheral address b = Bit number:
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))
//BB fuer PB1
#define TempSens *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,8)))

/*______________________________ PROTOTYPES __________________________________*/
void init_ports(void);

/*____________________________ Unterprogramme ________________________________*/
void init_ports(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //enable CLK for GPIOB
	GPIOB->CRL &= ~0x0F0; //Konfig-Bits von PB1 loeschen
	GPIOB->CRL |= 0x030; //configure PB1 as GPOPP
}

int main(void)
{
	lcd_init();
	lcd_clear();
	
	init_ports();
	
	while(1);
}
