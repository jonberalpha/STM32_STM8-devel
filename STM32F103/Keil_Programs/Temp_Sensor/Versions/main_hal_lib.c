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
#include "LCD_DEM16217.h"
#include "DS18B20.h"
#include <stdio.h>
#include <string.h>

/*_________________________________ MAIN _____________________________________*/
int main(void)
{
	char *LCDString={"Temp: "};
	char tempbuf[20];
	
	lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
	lcd16x2_clrscr();
	
	Ds18b20_Init();
	ds18b20[0].Temperature = 0;
	while(1)
	{
		Ds18b20_ManualConvert();
		sprintf(tempbuf,"%f",ds18b20[0].Temperature);
		strcat(LCDString,tempbuf);
		
		lcd16x2_puts(LCDString);
		wait_ms(500);
		lcd16x2_clrscr();
	}
}
