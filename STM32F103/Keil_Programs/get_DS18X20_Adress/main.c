/******************************************************************************/
/*   (C) Copyright Private - 2009-2018 All rights reserved. AUSTRIA           */
/*                                                                            */
/* File Name:   main.c                                                        */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        04.07.2020                                                    */
/* Description: 1-wire Temperatursensoren auf BluePill ansteueren             */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/

/*_______________________________ INCLUDES ___________________________________*/
#include "WAIT_STM32F10X.h"
#include "USART_STM32F10X.h"
#include "ONEWIRE_STM32F10X.h"

#include <stdio.h>
#include <string.h>

/*_________________________________ MAIN _____________________________________*/
int main(void)
{
  char buffer[40];
  uint8_t rom_c[8];
  int i=0;
  
  ow_init(GPIOB,0);
  uart_init(9600);
  wait_init();
  
  while(1)
  {
    // Start mit Master-Reset-Impuls u. Abfrage: Slave presence
    ow_send_mri();
    
    // Abfrage-Befehl senden: "READ ROM" = 0x33
    ow_write_byte(0x33);
    // Antwort einlesen: 8 Byte = 64 Bit Identifier ins
    // globale Array rom_c[.]
    for(i=0;i<8;i++)
    {
      rom_c[i] = ow_read_byte();
    }
    
    sprintf(buffer,"%02X %02X %02X %02X %02X %02X %02X %02X "
    "(HEX)\r\r",rom_c[7],rom_c[6],rom_c[5],rom_c[4],rom_c[3],
    rom_c[2],rom_c[1],rom_c[0]);
    
    //UART-Ausgabe
    uart_put_string(buffer);
    wait_ms(1000);
  }
}
