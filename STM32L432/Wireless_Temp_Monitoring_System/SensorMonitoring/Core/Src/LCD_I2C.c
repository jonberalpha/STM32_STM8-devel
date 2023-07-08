/**
 *******************************************************************************
 * @file    LCD_I2C.c
 * @authors Berger, Ibrahim, Markovic, Meyerowitsch
 * @brief   This file provides code to control the LCD-Display via the
 *          I2C-Module
 ******************************************************************************
 */

#include "LCD_I2C.h"
#include "stm32l4xx_hal.h"

/**
 * @name External declarations
 * @{
 */

/** @brief External Declaration of I2C1 handle */
extern I2C_HandleTypeDef hi2c1;

/**
 * @}
 */

/**
 * @name Definitions
 * @{
 */

/** @brief default address, also 0x4E is possible than shifting the adress is not necessary */
#define SLAVE_ADDRESS_LCD 0x27

/**
 * @}
 */

/**
 * @brief	Send command to the LCD-Display
 * @param	(char)cmd
 * @retval	none
 */
void lcd_i2c_send_cmd(char cmd)
{
  //int i=0;
  char data_u, data_l;
  uint8_t dataArray[4];

  data_u = (cmd & 0xf0);
  data_l = ((cmd << 4) & 0xf0);
  dataArray[0] = data_u | 0x0C;  //en=1, rs=0
  dataArray[1] = data_u | 0x08;  //en=0, rs=0
  dataArray[2] = data_l | 0x0C;  //en=1, rs=0
  dataArray[3] = data_l | 0x08;  //en=0, rs=0

  HAL_I2C_Master_Transmit(&hi2c1, (SLAVE_ADDRESS_LCD << 1), (uint8_t *)dataArray, sizeof(dataArray), 10U);
}

/**
 * @brief	Send data to the LCD-Display
 * @param	(char)data - Data which should be sent to the LCD-Display
 * @retval	none
 */
void lcd_i2c_send_data(char data)
{
  //int i=0;
  char data_u, data_l;
  uint8_t dataArray[4];

  data_u = (data&0xf0);
  data_l = ((data<<4)&0xf0);
  dataArray[0] = data_u|0x0D;  //en=1, rs=0
  dataArray[1] = data_u|0x09;  //en=0, rs=0
  dataArray[2] = data_l|0x0D;  //en=1, rs=0
  dataArray[3] = data_l|0x09;  //en=0, rs=0

  HAL_I2C_Master_Transmit(&hi2c1, (SLAVE_ADDRESS_LCD << 1), (uint8_t *)dataArray, sizeof(dataArray), 10U);
}

/**
 * @brief	Clears the LCD-Display
 * @param	none
 * @retval	none
 */
void lcd_i2c_clear(void)
{
  lcd_i2c_send_cmd(0x80);
  for(int i=0; i<70; i++)
  {
    lcd_i2c_send_data(' ');
  }
}

/**
 * @brief	Moves cursor on display-position according to given row and column
 * @param	(int)row - Number of row
 * @param	(int)col - Number of column
 * @retval	none
 */
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

/**
 * @brief	Initializes I2C-Module
 * @param   none
 * @retval  none
 */
void lcd_i2c_init(void)
{
  // In the initialization HAL_Delay is used because code will be executed before the FreeRTOS-Kernel starts
  // 4 bit initialisation
  HAL_Delay(50);              // wait for >40ms
  lcd_i2c_send_cmd(0x30);
  HAL_Delay(5);               // wait for >4.1ms
  lcd_i2c_send_cmd(0x30);
  HAL_Delay(1);               // wait for >100us
  lcd_i2c_send_cmd(0x30);
  HAL_Delay(10);
  lcd_i2c_send_cmd(0x20);     // 4bit mode
  HAL_Delay(10);

  // display initialisation
  lcd_i2c_send_cmd(0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
  HAL_Delay(1);
  lcd_i2c_send_cmd(0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
  HAL_Delay(1);
  lcd_i2c_send_cmd(0x01); // clear display
  HAL_Delay(1);
  lcd_i2c_send_cmd(0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
  HAL_Delay(1);
  lcd_i2c_send_cmd(0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

/**
 * @brief	Sends given string to LCD-Display
 * @param	(char*)str - String which should be written to LCD-Display
 * @retval	none
 */
void lcd_i2c_send_string(char *str)
{
  while(*str) lcd_i2c_send_data(*str++);
}
