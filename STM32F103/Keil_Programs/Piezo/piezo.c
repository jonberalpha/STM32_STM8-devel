/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   uart.c                                                        */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        30/04/2018                                                    */
/* Description: Demoprogramm fuer UART                                        */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#include "armv10_std.h"


int main(void)
{
  init_leds_switches();
  lcd_init();
  
  lcd_clear();
  lcd_set_cursor(0,0);
  lcd_put_string("Piezo Programm");
  
  while(1)
  {
    if((~get_switches()&0x01)==0x01)
    {
      lcd_set_cursor(1,0);
      lcd_put_string("Running...    ");
      ton(ton_A1,halbe);
    }
    lcd_set_cursor(1,0);
    lcd_put_string("Stopped!     ");
  }
}
