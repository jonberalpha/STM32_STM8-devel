/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   LCD_DEM16217.c                                                */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        13.05.2020                                                    */
/* Description: diverse Funktionen fuer das DEM16217 LCD Display              */
/*              Header-File stellt defines und Prototypen zur Verfuegung      */
/*              Befehle sind hier ausprogrammiert                             */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef LCD_DEM16217_H_INCLUDED
#define LCD_DEM16217_H_INCLUDED

/*________________________________ INCLUDES __________________________________*/
#include "stm32f10x.h"
#include "WAIT_STM32F10X.h"

#ifdef  LCD_DEM16217_MOD
#define EXPORT
#else
#define EXPORT extern
#endif 

/** Port and pin definition for 4-bit mode ---------------------------------- */
// LCD control lines (must be on the same port) 
#define LCD16X2_RCC_GPIO_CONTROL  RCC_APB2Periph_GPIOC
#define LCD16X2_GPIO_CONTROL      GPIOC
#define LCD16X2_GPIO_RS            GPIOC
#define LCD16X2_GPIO_RW            GPIOC
#define LCD16X2_GPIO_EN            GPIOC
// LCD data lines (must be on the same port)
#define LCD16X2_RCC_GPIO_DATA      RCC_APB2Periph_GPIOC
#define LCD16X2_GPIO_DATA          GPIOC
#define LCD16X2_GPIO_D4            GPIOC
#define LCD16X2_GPIO_D5            GPIOC
#define LCD16X2_GPIO_D6            GPIOC
#define LCD16X2_GPIO_D7            GPIOC
// Pin definition
#define LCD16X2_PIN_RS            0x1000
#define LCD16X2_PIN_RW            0x0800
#define LCD16X2_PIN_EN            0x0400
#define LCD16X2_PIN_D4            0x0008    // 4-bit mode LSB
#define LCD16X2_PIN_D5            0x0004
#define LCD16X2_PIN_D6            0x0002
#define LCD16X2_PIN_D7            0x0001    // 4-bit mode MSB

/** Display size ------------------------------------------------------------ */
// Number of visible lines of the display (1 or 2)
#define LCD16X2_LINES          2
// Visible characters per line of the display
#define LCD16X2_DISP_LENGTH    16
// DDRAM address of first char of line 1
#define LCD16X2_START_LINE_1  0x00
// DDRAM address of first char of line 2
#define LCD16X2_START_LINE_2  0x40

/** Delay value ------------------------------------------------------------- */
// Delay power on
#define LCD16X2_DELAY_POWER_ON       16000
// Delay after initialize 8-bit
#define LCD16X2_DELAY_INIT          5000
// Delay after initialize 8-bit repeated
#define LCD16X2_DELAY_INIT_REP      64
// Delay after set 4-bit mode
#define LCD16X2_DELAY_INIT_4BIT     64
// Delay until address counter updated after busy flag is cleared
#define LCD16X2_DELAY_BUSY_FLAG      4
// Enable pulse width high level
#define LCD16X2_DELAY_ENABLE_PULSE  2

/** Instructions bit location ----------------------------------------------- */
#define LCD16X2_CLEAR_DISPLAY          0x01
#define LCD16X2_CURSOR_HOME            0x02
#define LCD16X2_CHARACTER_ENTRY_MODE  0x04
#define LCD16X2_DISPLAY_CURSOR_ON_OFF  0x08
#define LCD16X2_DISPLAY_CURSOR_SHIFT   0x10
#define LCD16X2_FUNCTION_SET          0x20
#define LCD16X2_SET_CGRAM_ADDRESS       0x40
#define LCD16X2_SET_DDRAM_ADDRESS       0x80
/* Character entry mode instructions */
#define LCD16X2_INCREMENT              0x02  // Initialization setting
#define LCD16X2_DECREMENT              0x00
#define LCD16X2_DISPLAY_SHIFT_ON      0x01
#define LCD16X2_DISPLAY_SHIFT_OFF      0x00  // Initialization setting
/* Display cursor on off instructions */
#define LCD16X2_DISPLAY_ON             0x04
#define LCD16X2_DISPLAY_OFF             0x00  // Initialization setting
#define LCD16X2_CURSOR_UNDERLINE_ON     0x02
#define LCD16X2_CURSOR_UNDERLINE_OFF  0x00  // Initialization setting
#define LCD16X2_CURSOR_BLINK_ON         0x01
#define LCD16X2_CURSOR_BLINK_OFF       0x00  // Initialization setting
/* Display cursor shift instructions */
#define LCD16X2_DISPLAY_SHIFT          0x08
#define LCD16X2_CURSOR_MOVE            0x00
#define LCD16X2_RIGHT_SHIFT            0x04
#define LCD16X2_LEFT_SHIFT            0x00
/* Function set instructions */
#define LCD16X2_8BIT_INTERFACE        0x10  // Initialization setting
#define LCD16X2_4BIT_INTERFACE        0x00
#define LCD16X2_2LINE_MODE            0x08
#define LCD16X2_1LINE_MODE            0x00  // Initialization setting
#define LCD16X2_5X10DOT_FORMAT        0x04
#define LCD16X2_5X7DOT_FORMAT          0x00  // Initialization setting
/* Busy flag bit location */
#define LCD16X2_BUSY_FLAG              0x80

/** LCD display and cursor attributes --------------------------------------- */
#define LCD16X2_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF  (LCD16X2_DISPLAY_OFF | \
  LCD16X2_CURSOR_UNDERLINE_OFF | LCD16X2_CURSOR_BLINK_OFF)
#define LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF    (LCD16X2_DISPLAY_ON | \
  LCD16X2_CURSOR_UNDERLINE_OFF | LCD16X2_CURSOR_BLINK_OFF)          
#define LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_ON    (LCD16X2_DISPLAY_ON | \
  LCD16X2_CURSOR_UNDERLINE_OFF | LCD16X2_CURSOR_BLINK_ON)
#define LCD16X2_DISPLAY_ON_CURSOR_ON_BLINK_OFF    (LCD16X2_DISPLAY_ON | \
  LCD16X2_CURSOR_UNDERLINE_ON | LCD16X2_CURSOR_BLINK_OFF)
#define LCD16X2_DISPLAY_ON_CURSOR_ON_BLINK_ON    (LCD16X2_DISPLAY_ON | \
  LCD16X2_CURSOR_UNDERLINE_ON | LCD16X2_CURSOR_BLINK_ON)

/*____________________________ EXPORTED FUNCTIONS ____________________________*/
EXPORT void lcd16x2_init(uint8_t disp_attr);
EXPORT void lcd16x2_write_command(uint8_t cmd);
EXPORT void lcd16x2_write_data(uint8_t data);
EXPORT void lcd16x2_clrscr(void);
EXPORT void lcd16x2_home(void);
EXPORT void lcd16x2_gotoxy(uint8_t x, uint8_t y);
EXPORT uint8_t lcd16x2_getxy(void);
EXPORT void lcd16x2_entry_inc(void);
EXPORT void lcd16x2_entry_dec(void);
EXPORT void lcd16x2_entry_inc_shift(void);
EXPORT void lcd16x2_entry_dec_shift(void);
EXPORT void lcd16x2_display_on(void);
EXPORT void lcd16x2_display_off(void);
EXPORT void lcd16x2_cursor_on(void);
EXPORT void lcd16x2_cursor_off(void);
EXPORT void lcd16x2_blink_on(void);
EXPORT void lcd16x2_blink_off(void);
EXPORT void lcd16x2_display_shift_left(void);
EXPORT void lcd16x2_display_shift_right(void);
EXPORT void lcd16x2_cursor_shift_left(void);
EXPORT void lcd16x2_cursor_shift_right(void);
EXPORT void lcd16x2_putc(const char c);
EXPORT void lcd16x2_puts(const char* s);
EXPORT void lcd16x2_create_custom_char(uint8_t location, uint8_t* data_bytes);
EXPORT void lcd16x2_put_custom_char(uint8_t x, uint8_t y, uint8_t location);

#undef EXPORT
#endif /* __LCD_DEM16217_STM32F10X_H */
/***************************  (C) PRIVATE  2020  *********END OF FILE**********/
