/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
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
#include "ONEWIRE_STM32F10X.h"
#include "USART_STM32F10X.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

/*_______________________________ DEFINES ____________________________________*/
#define DEVICE_AMOUNT     2
#define TEMP_RESOLUTION  12

/*______________________________ STRUCTURES __________________________________*/
typedef struct DS18X20
{
  int index;
  uint8_t ROM_CODE[8];
  float temperature;
}DS18X20;

/*__________________________ GLOBALE VARIABLES _______________________________*/
DS18X20 devices[DEVICE_AMOUNT]; //Deklaration
uint8_t ROM_CODES[DEVICE_AMOUNT][8] =
{
  {0x26, 0x01, 0x19, 0x3B, 0x5A, 0x38, 0xB3, 0x28},
  {0x31, 0x01, 0x19, 0x3B, 0x5A, 0x73, 0x6E, 0x28}
};

/*______________________________ PROTOTYPES __________________________________*/
float conv_temp(uint8_t datareg[]);
void define_struct(void);

/*______________________________ SUBROUTINES _________________________________*/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    conv_temp()                       */
/*                                                                            */
/* Aufgabe:   Convert Temperature                                             */
/*            convert sensordata to temperature                               */
/*                                                                            */
/*            Temperature Register:                                           */
/*                                                                            */
/*             7    6    5    4    3     2      1     0                       */
/* LS BYTE    2^3  2^2  2^1  2^0  2^-1  2^-2  2^-3  2^-4   datareg[0]         */
/*            15   14   13   12    11    10     9     8                       */
/* MS BYTE     S    S    S    S    S    2^6    2^5   2^4   datareg[1]         */
/*                                                                            */
/* Input:     uint8_t datareg[]                                               */
/* return:    float ftemp                                                     */
/******************************************************************************/
float conv_temp(uint8_t datareg[])
{
  uint8_t sign = datareg[1] & 0x80;
  int16_t temp = (datareg[1] << 8) + datareg[0];

  if(sign)
  {
    temp = ((temp ^ 0xffff) + 1) * -1;
  }
  
  return temp/16.0;
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    define_struct()                   */
/*                                                                            */
/* Aufgabe:   definiert das Sensor-Array mit den entsprechenden ROM_Codes     */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void define_struct(void)
{
  int i=0,j=0;
  
  for(i=0;i<DEVICE_AMOUNT;i++)
  {
    devices[i].index = i;
    for(j=0;j<8;j++)
    {
      devices[i].ROM_CODE[j] = ROM_CODES[i][j];
    }
  }
}

/*_________________________________ MAIN _____________________________________*/
int main(void)
{
  char buffer[20];
  int i=0,j=0;
  uint8_t datareg[9];
  
  ow_init(GPIOB,0);
  uart_init(9600);
  wait_init();
  
  define_struct(); //definiert das Sensor-Array mit den entsprechenden ROM_Codes
  
  while(1)
  {
    ow_send_mri();       //start conversation
    ow_write_byte(0xCC); //skip ROM -> alle Sensoren werden angesprochen
    ow_write_byte(0x44); //convert T
    
    wait_ms(1000);    //mind. 750ms Messdauer abwarten
    
    for(i=0;i<DEVICE_AMOUNT;i++)
    {
      ow_send_mri();       //start conversion
      ow_write_byte(0x55); //match ROM
      for(j=0;j<8;j++)
      {
        ow_write_byte(devices[i].ROM_CODE[7-j]);
      }
      ow_write_byte(0xBE); //Temperaturwert auslesen
      for(j=0;j<9;j++)  //Antwort auslesen: 9 Byte grosse ScratchPad-Inhalt einlesen
      {
        datareg[j] = ow_read_byte();
      }
      
      devices[i].temperature = conv_temp(datareg);
      
      //UART-Ausgabe
      sprintf(buffer,"%d.SENSOR: %9.4f Grad Celsius\r",devices[i].index + 1,devices[i].temperature);
      uart_put_string(buffer);
      if(i == DEVICE_AMOUNT-1)
      {
        uart_put_string("\r");
      }
      wait_ms(1000);
    }
  }
}

