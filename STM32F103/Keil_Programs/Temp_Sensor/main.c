/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   main.c                                                        */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        08.06.2019                                                    */
/* Description: 1-wire Temperatursensor auf dem Minimalsystem (PB1) ansteueren*/
/*              DS18S20                                                       */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/

/*_______________________________ INCLUDES ___________________________________*/
#include "WAIT_STM32F10X.h"
#include "LCD_DEM16217.h"
#include "SENS_DS18X20.h"
#include <stdio.h>
#include <string.h>

/*_________________________________ MAIN _____________________________________*/
int main(void)
{
  float temp=0;
  char buffer[20];
  
  lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
  lcd16x2_clrscr();
  
  DS18X20_init();
  
  while(1)
  {
    temp = get_temperature();
    
    //LCD-Ausgabe
    sprintf(buffer,"Temp: %5.1f oC",temp);
    lcd16x2_puts(buffer);
    lcd16x2_home();
    wait_ms(500);
    
  }
}
