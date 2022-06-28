/******************************************************************************/
/* File Name:   USART_STM32L432XX.h                                           */
/* Autor: 	    Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        28.09.2021                                                    */
/* Description: USART_STM32L432XX Library                                     */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef USART_STM32L432XX_H_INCLUDED
#define USART_STM32L432XX_H_INCLUDED

/*_______________________________ INCLUDES ___________________________________*/
#include <stm32l432xx.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef  USART_STM32L432XX_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/*________________________________ DEFINES ___________________________________*/
#define ASCII_ESC    27     // \033 or \x1b | CMD Start
#define ASCII_CLRSCR "[2J"  // whole cmd-string: \033[2J; \x1b[2J
#define ASCII_HOME   "[H"
#define ASCII_SVRPOS "[s"
#define ASCII_RESPOS "[u"

/*___________________________ EXPORTED FUNCTIONS _____________________________*/
/******************************************************************************/
/*            SUBROUTINE:    uart_init(unsigned long baud)                    */
/*                                                                            */
/* Purpose:   USART1 PA9 und PA10 initialisieren                              */
/* Input:     BAUDRATE                                                        */
/* return:	                                                                  */
/******************************************************************************/
EXPORT void uart_init(unsigned long baud);

/******************************************************************************/
/*            SUBROUTINE:    uart_clear(void)                                 */
/*                                                                            */
/* Purpose:   Screen clear                                                    */
/* Input:                                                                     */
/* return:	                                                                  */
/******************************************************************************/
EXPORT void uart_clear(void);

/******************************************************************************/
/*            SUBROUTINE:    uart_put_char(char c)                            */
/*                                                                            */
/* Purpose:   Zeichen senden                                                  */
/* Input:     char                                                            */
/* return:	                                                                  */
/******************************************************************************/
EXPORT void uart_put_char(char ch);

/******************************************************************************/
/*            SUBROUTINE:    uart_put_string(char *string)                    */
/*                                                                            */
/* Purpose:   Zeichenkette/String senden                                      */
/* Input:     string                                                          */
/* return:	                                                                  */
/******************************************************************************/
EXPORT void uart_put_string(char *string);

/******************************************************************************/
/*            SUBROUTINE:    nib2asc(char *nib)                               */
/*                                                                            */
/* Purpose:   Function converts a Nibble(0-F) to an ASCII ('0'-'F')           */
/* Input:     nib: Nibble to convert                                          */
/* Output:    nib: Converted Nibble                                           */
/* return:                                                                    */
/******************************************************************************/
EXPORT void nib2asc(char *nib);

/******************************************************************************/
/*            SUBROUTINE:    uart_put_hex(char c)                             */
/*                                                                            */
/* Purpose:   Ausgabe eines 8 Bit Hex Wertes als ASCII String auf UART1       */
/* Input:     char c                                                          */
/* Output:                                                                    */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_put_hex(char ch);

/******************************************************************************/
/*            SUBROUTINE:    uart_get_char(void)                              */
/*                                                                            */
/* Purpose:   Zeichen empfangen                                               */
/* Input:                                                                     */
/* return:	  char                                                            */
/******************************************************************************/
EXPORT char uart_get_char(void);

/******************************************************************************/
/*            SUBROUTINE:    uart_get_string(char *string)                    */
/*                                                                            */
/* Purpose:   String empfangen                                                */
/* Input:     char *buffer                                                    */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_get_string(char *string);

/******************************************************************************/
/*            SUBROUTINE:    uart_saveCursorPos(void)                         */
/*                                                                            */
/* Purpose:   Cursor Position speichern                                       */
/* Input:                                                                     */
/* return:	                                                                  */
/******************************************************************************/
EXPORT void uart_saveCursorPos(void);

/******************************************************************************/
/*            SUBROUTINE:    uart_restoreCursorPos(void)                      */
/*                                                                            */
/* Purpose:   Cursor Position wiederherstellen                                */
/* Input:                                                                     */
/* return:	                                                                  */
/******************************************************************************/
EXPORT void uart_restoreCursorPos(void);

/******************************************************************************/
/*            SUBROUTINE:    uart_setpos(char x, char y)                      */
/*                                                                            */
/* Purpose:   Cursor Position setzen                                          */
/* Input:     char x, char y                                                  */
/* return:	                                                                  */
/******************************************************************************/
EXPORT void uart_setpos(char x,char y);

/******************************************************************************/
/*            SUBROUTINE:  uart_printf(const char *format, ...)               */
/*                                                                            */
/* Purpose:   uart ausgeben                                                   */
/* Input:     Format uebergeben                                               */
/* return:	                                                                  */
/******************************************************************************/
EXPORT void uart_printf(const char *format, ...);

#undef EXPORT
#endif /* __USART_STM32L432XX_H */
