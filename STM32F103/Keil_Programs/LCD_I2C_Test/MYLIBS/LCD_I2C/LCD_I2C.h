/******************************************************************************/
/*   (C) Copyright - PRIVATE  2020 All rights reserved. AUSTRIA               */ 
/*                                                                            */ 
/* File Name:   LCD_I2C.h                                                     */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        25.08.2020                                                    */
/* Description: LCD-I2C Library                                               */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef LCD_I2C_H_INCLUDED
#define LCD_I2C_H_INCLUDED

/*_______________________________ INCLUDES ___________________________________*/
#include <stm32f10x.h>
#include "I2C_STM32F10X.h"
#include "WAIT_STM32F10X.h"

#ifdef  LCD_I2C_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/*___________________________ EXPORTED FUNCTIONS _____________________________*/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    lcd_i2c_send_cmd()                */
/*                                                                            */
/* Aufgabe:   send command to LCD display                                     */
/* Input:     char cmd                                                        */
/* return:                                                                    */
/******************************************************************************/
EXPORT void lcd_i2c_send_cmd(char cmd);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    lcd_i2c_send_data()               */
/*                                                                            */
/* Aufgabe:   send data to LCD display                                        */
/* Input:     char data                                                       */
/* return:                                                                    */
/******************************************************************************/
EXPORT void lcd_i2c_send_data(char data);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    lcd_i2c_clear()                   */
/*                                                                            */
/* Aufgabe:   send clear screen to LCD display                                */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void lcd_i2c_clear(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    lcd_i2c_put_curs()                */
/*                                                                            */
/* Aufgabe:   set cursor location                                             */
/* Input:     int row, int col                                                */
/* return:                                                                    */
/******************************************************************************/
EXPORT void lcd_i2c_put_curs(int row, int col);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    lcd_i2c_init()                    */
/*                                                                            */
/* Aufgabe:   initialize LCD display                                          */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void lcd_i2c_init(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    lcd_i2c_send_string()             */
/*                                                                            */
/* Aufgabe:   send string to LCD display                                      */
/* Input:     char *str                                                       */
/* return:                                                                    */
/******************************************************************************/
EXPORT void lcd_i2c_send_string(char *str);

#undef EXPORT
#endif /* __LCD_I2C_H */
/***************************  (C) PRIVATE  2020  *********END OF FILE*********/
