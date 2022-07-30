/******************************************************************************/
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

/*___________________________ EXPORTED FUNCTIONS ____________________________*/
/******************************************************************************/
/*            SUBROUTINE:    lcd_i2c_send_cmd()                               */
/*                                                                            */
/* Purpose:   send command to LCD display                                     */
/* Input:     char cmd                                                        */
/* return:                                                                    */
/******************************************************************************/
EXPORT void lcd_i2c_send_cmd(char cmd);

/******************************************************************************/
/*            SUBROUTINE:    lcd_i2c_send_data()                              */
/*                                                                            */
/* Purpose:   send data to LCD display                                        */
/* Input:     char data                                                       */
/* return:                                                                    */
/******************************************************************************/
EXPORT void lcd_i2c_send_data(char data);

/******************************************************************************/
/*            SUBROUTINE:    lcd_i2c_clear()                                  */
/*                                                                            */
/* Purpose:   send clear screen to LCD display                                */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void lcd_i2c_clear(void);

/******************************************************************************/
/*            SUBROUTINE:    lcd_i2c_put_curs()                               */
/*                                                                            */
/* Purpose:   set cursor location                                             */
/* Input:     int row, int col                                                */
/* return:                                                                    */
/******************************************************************************/
EXPORT void lcd_i2c_put_curs(int row, int col);

/******************************************************************************/
/*            SUBROUTINE:    lcd_i2c_init()                                   */
/*                                                                            */
/* Purpose:   initialize LCD display                                          */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void lcd_i2c_init(void);

/******************************************************************************/
/*            SUBROUTINE:    lcd_i2c_send_string()                            */
/*                                                                            */
/* Purpose:   send string to LCD display                                      */
/* Input:     char *str                                                       */
/* return:                                                                    */
/******************************************************************************/
EXPORT void lcd_i2c_send_string(char *str);

#undef EXPORT
#endif /* __LCD_I2C_H */
