/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   LCD_I2C.c                                                     */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        25.08.2020                                                    */
/* Description: LCD-I2C Library                                               */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define LCD_I2C_MOD

/*_______________________________ INCLUDES ___________________________________*/
#include "LCD_I2C.h"

/*_______________________________ DEFINES ____________________________________*/
#define SLAVE_ADDRESS_LCD 0x27 //default address

/*_____________________________ UNTERPROGRAMME _______________________________*/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    lcd_i2c_send_cmd()                */
/*                                                                            */
/* Aufgabe:   send command to LCD display                                     */
/* Input:     char cmd                                                        */
/* return:                                                                    */
/******************************************************************************/
void lcd_i2c_send_cmd(char cmd)
{
  //int i=0;
  char data_u, data_l;
  uint8_t data_t[4];

  data_u = (cmd & 0xf0);
  data_l = ((cmd << 4) & 0xf0);
  data_t[0] = data_u | 0x0C;  //en=1, rs=0
  data_t[1] = data_u | 0x08;  //en=0, rs=0
  data_t[2] = data_l | 0x0C;  //en=1, rs=0
  data_t[3] = data_l | 0x08;  //en=0, rs=0

  i2c_communicate(I2C1,SLAVE_ADDRESS_LCD,(uint8_t*) data_t,4,0,0);
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    lcd_i2c_send_data()               */
/*                                                                            */
/* Aufgabe:   send data to LCD display                                        */
/* Input:     char data                                                       */
/* return:                                                                    */
/******************************************************************************/
void lcd_i2c_send_data(char data)
{
  //int i=0;
  char data_u, data_l;
  uint8_t data_t[4];
  
  data_u = (data&0xf0);
  data_l = ((data<<4)&0xf0);
  data_t[0] = data_u|0x0D;  //en=1, rs=0
  data_t[1] = data_u|0x09;  //en=0, rs=0
  data_t[2] = data_l|0x0D;  //en=1, rs=0
  data_t[3] = data_l|0x09;  //en=0, rs=0
  
  i2c_communicate(I2C1,SLAVE_ADDRESS_LCD,(uint8_t*) data_t,4,0,0);
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    lcd_i2c_clear()                   */
/*                                                                            */
/* Aufgabe:   send clear screen to LCD display                                */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void lcd_i2c_clear(void)
{
  lcd_i2c_send_cmd(0x80);
  for (int i=0; i<70; i++)
  {
    lcd_i2c_send_data(' ');
  }
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    lcd_i2c_put_curs()                */
/*                                                                            */
/* Aufgabe:   set cursor location                                             */
/* Input:     int row, int col                                                */
/* return:                                                                    */
/******************************************************************************/
void lcd_i2c_put_curs(int row, int col)
{
  switch(row)
  {
    case 0:
      col |= 0x80;
      break;
    case 1:
      col |= 0xC0;
      break;
  }
  
  lcd_i2c_send_cmd(col);
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    lcd_i2c_init()                    */
/*                                                                            */
/* Aufgabe:   initialize LCD display                                          */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void lcd_i2c_init(void)
{
  //init wait
  wait_init();
  
  //init I2C1, no pin remapping, no fast mode, APB1 clock is 8 MHz
  i2c_init(I2C1, true, false, 8000000);
  
  // 4 bit initialisation
  wait_ms(50);              // wait for >40ms
  lcd_i2c_send_cmd(0x30);
  wait_ms(5);               // wait for >4.1ms
  lcd_i2c_send_cmd(0x30);
  wait_ms(1);               // wait for >100us
  lcd_i2c_send_cmd(0x30);
  wait_ms(10);
  lcd_i2c_send_cmd(0x20);   // 4bit mode
  wait_ms(10);

  // dislay initialisation
  lcd_i2c_send_cmd(0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
  wait_ms(1);
  lcd_i2c_send_cmd(0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
  wait_ms(1);
  lcd_i2c_send_cmd(0x01); // clear display
  wait_ms(1);
  wait_ms(1);
  lcd_i2c_send_cmd(0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
  wait_ms(1);
  lcd_i2c_send_cmd(0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    lcd_i2c_send_string()             */
/*                                                                            */
/* Aufgabe:   send string to LCD display                                      */
/* Input:     char *str                                                       */
/* return:                                                                    */
/******************************************************************************/
void lcd_i2c_send_string(char *str)
{
  while(*str) lcd_i2c_send_data(*str++);
}
