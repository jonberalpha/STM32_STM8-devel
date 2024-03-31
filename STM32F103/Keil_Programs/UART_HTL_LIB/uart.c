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
  uart_init(9600);
  uart_clear();
  while(1) 
  {
    uart_put_char(uart_get_char());
  }
}
