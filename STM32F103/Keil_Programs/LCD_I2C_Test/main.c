/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   main.c                                                        */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        08.06.2019                                                    */
/* Description: LCD-Ansteuerung mittels I2C Portexpender                      */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/

/*_______________________________ INCLUDES ___________________________________*/
#include "WAIT_STM32F10X.h"
#include "LCD_I2C.h"
#include <stdio.h>
#include <string.h>

/*_________________________________ MAIN _____________________________________*/
int main(void)
{
  int i=0,row=0,col=0;
  
  wait_init();
  lcd_i2c_init();
  lcd_i2c_clear();
  
  while(1)
  {
    for (i=0;i<128;i++)
    {
      lcd_i2c_put_curs(row, col);
      lcd_i2c_send_data(i+48);

      col++;

      if(col > 15)
      {
        row++; 
        col = 0;
      }
      
      if(row > 1)
      {        
        row=0;
      }

      wait_ms(250);
    }
  }
}
