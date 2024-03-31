/******************************************************************************/
/*   (C) Copyright Private  2009-2018 All rights reserved. AUSTRIA            */ 
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
#include "WAIT_STM32F10X.h"
#include "LCD_DEM16217.h"
#include "DS18X20.h"
#include <stdio.h>
#include <string.h>

/*_______________________________ DEFINES ____________________________________*/
#define ANZBYTES 9

/*___________________________ GLOBALE VARIABLES ______________________________*/
uint8_t datareg[ANZBYTES];

/*_________________________________ MAIN _____________________________________*/
int main(void)
{
	float temp=0;
	char buffer[20];
	char *LCDString={"Temp: "};
	char tempbuf[20];
	
	OWire Sens; //define Temperatur Sensor
	
	wait_init();
	lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
	lcd16x2_clrscr();
	
	OWInit(&Sens);
	
	while(1)
	{
		OWReset(&Sens);
		OWSkip(&Sens);
		
		OWWrite(&Sens,ConvertTemp);
		wait_ms(750);
		
		OWReset(&Sens);
		OWSkip(&Sens);
		
		OWRead_bytes(&Sens,datareg,ANZBYTES);
		
		temp=convTemp(datareg);
		
		// LCD-Ausgabe
		sprintf(tempbuf,"%5.1f",temp);
		strcpy(buffer,tempbuf);
		
		lcd16x2_puts(LCDString);
		lcd16x2_home();
		
	}
}
