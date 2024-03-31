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

#define ANZBYTES 9

/*_________________________________ MAIN _____________________________________*/
int main(void)
{
	//float temp=0;
	int i=0;
	char *buffer;
	char *LCDString={"Temp: "};
	char *tempbuf;
	
	OWire Sens; //define Temperatur Sensor
	uint8_t rawdata[ANZBYTES]; //Rohdaten Temperatur
	
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
		
		OWRead_bytes(&Sens,rawdata,ANZBYTES);
		for(i=0;i<=ANZBYTES;i++)
		{
			sprintf(tempbuf,"%u",rawdata[i]);
			strcpy(buffer,tempbuf);
		}
		
		strcat(LCDString,buffer);
		
		lcd16x2_puts(LCDString);
		//temp=convTemp(&rawdata);
		
	}
}
