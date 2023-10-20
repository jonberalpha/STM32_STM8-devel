/******************************************************************************/
/* File Name:   EA_DOGM204_A.h                                                */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        18.02.2018                                                    */
/* Description: EA DOGM204-A LCD Display interfacing                          */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef EA_DOGM204_A_H_INCLUDED
#define EA_DOGM204_A_H_INCLUDED

/* -----------------------------------Includes -------------------------------*/
#include <stm32f10x.h>

#ifdef  EA_DOGM204_A_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/* ----------------------------Exported functions --------------------------- */

/******************************************************************************/
/*            SUBROUTINE:    Delay(ms)                                        */
/*                                                                            */
/* Purpose:   Wait for x ms                                                   */
/* Input:     Time in ms                                                      */
/* return:                                                                    */
/******************************************************************************/
EXPORT void Delay(int);

/******************************************************************************/
/*            SUBROUTINE:    LCD_Init()                                       */
/*                                                                            */
/* Purpose:   Initialize LCD-Display                                          */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_Init(void);

/******************************************************************************/
/*            SUBROUTINE:    LCD_PutChar(ASCII-Code)                          */
/*                                                                            */
/* Purpose:   Send char to display                                            */
/* Input:     Hex-value(integer) or direct-char of ASCII-character            */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_PutChar(char ascii);

/******************************************************************************/
/*            SUBROUTINE:    LCD_PutString(string)                            */
/*                                                                            */
/* Purpose:   Send string to display                                          */
/* Input:     String                                                          */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_PutString(char string[]);

/******************************************************************************/
/*            SUBROUTINE:    LCD_Cursor(offset)                               */
/*                                                                            */
/* Purpose:   Move cursor according to given offset                           */
/* Input:     value of offset                                                 */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_Cursor(int anz);

/******************************************************************************/
/*            SUBROUTINE:    LCD_Clear()                                      */
/*                                                                            */
/* Purpose:   Clear display content                                           */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_Clear(void);

/******************************************************************************/
/*            SUBROUTINE:    LCD_SetPosition(postion)                         */
/*                                                                            */
/* Purpose:   Move to specific line                                           */
/* Input:     Macros: LINE1, LINE2, LINE3 or LINE4                            */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_SetPosition(char pos);

/******************************************************************************/
/*            SUBROUTINE:    LCD_SetView(view)                                */
/*                                                                            */
/* Purpose:   Change view: bottom view, top view                              */
/* Input:     Macros: BOTTOMVIEW or TOPVIEW                                   */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_SetView(char view);

/******************************************************************************/
/*            SUBROUTINE:    LCD_SetROM(rom)                                  */
/*                                                                            */
/* Purpose:   Select ROMA, ROMB, ROMC                                         */
/*            Difference in character-set (see datasheet)                     */
/* Input:     Macros: ROMA, ROMB or ROMC                                      */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_SetROM(char rom);

/******************************************************************************/
/*            SUBROUTINE:    LCD_WriteChar(character)                         */
/*                                                                            */
/* Purpose:   Send character to display                                       */
/* Input:     Char                                                            */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_WriteChar(char character);

/******************************************************************************/
/*            SUBROUTINE:    LCD_WriteString(string)                          */
/*                                                                            */
/* Purpose:   Send string to display                                          */
/* Input:     String                                                          */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_WriteString(char *string);

/*______________________________ BITBANDING __________________________________*/
#define GPIOB_ODR GPIOB_BASE + 3*sizeof(uint32_t) //Calc peripheral address GPIOB ODR
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))
#define RESET  *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,11))) //RESET/PB11
#define NSS   *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,12)))  //NSS/PB12

/*___________________________ CONSTANT DEFINITION ____________________________*/
#define HOME_L1     0x80
#define LINE1       0
#define LINE2       LINE1+0x20
#define LINE3       LINE1+0x40
#define LINE4       LINE1+0x60

#define DISPLAY_ON  0x04
#define DISPLAY_OFF 0x03
#define CURSOR_ON   0x02
#define CURSOR_OFF  0x05
#define BLINK_ON    0x01
#define BLINK_OFF   0x06

#define TOPVIEW     0x05
#define BOTTOMVIEW  0x06
#define ROMA        0x00
#define ROMB        0x04
#define ROMC        0x0C

#undef  EXPORT
#endif /* __EA_DOGM204_A_H */
