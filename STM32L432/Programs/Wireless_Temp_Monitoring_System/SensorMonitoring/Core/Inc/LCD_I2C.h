/**
 *******************************************************************************
 * @file    LCD_I2C.h
 * @authors Berger, Ibrahim, Markovic, Meyerowitsch
 * @brief   This file provides all function prototypes for the LCD_I2C.c file
 ******************************************************************************
 */

#ifndef LCD_I2C_H
#define LCD_I2C_H

void lcd_i2c_send_cmd(char cmd);
void lcd_i2c_send_data(char data);
void lcd_i2c_clear(void);
void lcd_i2c_put_curs(int row, int col);
void lcd_i2c_init(void);
void lcd_i2c_send_string(char *str);

#endif /* LCD_I2C_H_ */
