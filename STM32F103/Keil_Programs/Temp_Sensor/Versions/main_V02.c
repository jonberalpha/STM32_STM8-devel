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
#include <armv10_std.h>
#include "DS18X20.h"

/*_________________________________ MAIN _____________________________________*/
int main(void)
{
	int i=0;
	float temp=0;
	char buffer[20];
	
	lcd_init();
	lcd_clear();
	
	init_ports();
	initWait();
	
	while(1)
	{
		sendMRI();              // send MRI
		skipROM();              // simply calls all slaves instead of an specific address call
		
		writeByte(ConvertTemp);	// Temperaturmessung ausfuehren
		waitms(750);            // Messdauer abwarten
		
		sendMRI();
		skipROM();
		
		for(i=0;i<9;i++)    // Antwort einlesen: 9 Byte große Scratch Pad-Inhalt einlesen
		{
			datareg[i] = readByte();
		}
		temp=convTemp(datareg);
		
		// LCD-Ausgabe
		lcd_set_cursor(0,0);
		lcd_put_string("Temp (oC):     ");
		lcd_set_cursor(0,11);
		sprintf(&buffer[1],"%5.1f",temp);
		lcd_put_string(&buffer[1]);
		
		lcd_set_cursor(1,0);
		lcd_put_string("               ");
		sprintf(&buffer[1],"%s",datareg);
		lcd_set_cursor(1,0);
		lcd_put_string(&buffer[1]);
	}
}
