/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   USART_STM32F10X.h                                             */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        11.05.2020                                                    */
/* Description: USART Functions                                               */
/*              Header-File stellt defines und Prototypen zur Verfuegung      */
/*              Befehle sind hier ausprogrammiert                             */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef USART_STM32F10X_H_INCLUDED
#define USART_STM32F10X_H_INCLUDED

/*________________________________ INCLUDES __________________________________*/
#include <stm32f10x.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef  USART_STM32F10X_MOD
#define EXPORT
#else
#define EXPORT extern
#endif 

/*____________________________ EXPORTED FUNCTIONS ____________________________*/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_init(unsigned long baud)     */
/*                                                                            */
/* Aufgabe:   USART1 PA9 und PA10 initialisieren                              */
/* Input:     BAUDRATE                                                        */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_init(unsigned long baud);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_clear(void)                  */
/*                                                                            */
/* Aufgabe:   Screen clear                                                    */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_clear(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_put_char(char c)             */
/*                                                                            */
/* Aufgabe:   Zeichen senden                                                  */
/* Input:     char                                                            */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_put_char(char c);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_put_string(char *string)     */
/*                                                                            */
/* Aufgabe:   Zeichenkette/String senden                                      */
/* Input:     string                                                          */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_put_string(char *string);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    nib2asc(char *nib)                */
/*                                                                            */
/* Aufgabe:   Function converts a Nibble(0-F) to an ASCII ('0'-'F')           */
/* Input:     nib: Nibble to convert                                          */
/* Output:    nib: Converted Nibble                                           */
/* return:                                                                    */
/******************************************************************************/
EXPORT void nib2asc(char *nib);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_put_hex(char c)              */
/*                                                                            */
/* Aufgabe:   Ausgabe eines 8 Bit Hex Wertes als ASCII String auf UART1       */
/* Input:     char c                                                          */
/* Output:                                                                    */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_put_hex(char c);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_get_char(void)               */
/*                                                                            */
/* Aufgabe:   Zeichen empfangen                                               */
/* Input:                                                                     */
/* return:    char                                                            */
/******************************************************************************/
EXPORT char uart_get_char(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_saveCursorPos(void)          */
/*                                                                            */
/* Aufgabe:   Cursor Position speichern                                       */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_saveCursorPos(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_restoreCursorPos(void)       */
/*                                                                            */
/* Aufgabe:   Cursor Position wiederherstellen                                */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_restoreCursorPos(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_setpos(char x, char y)       */
/*                                                                            */
/* Aufgabe:   Cursor Position setzen                                          */
/* Input:     char x, char y                                                  */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_setpos(char x,char y);

/******************************************************************************/
/*            U N T E R P R O G R A M M:  uart_printf(const char *format, ...)*/
/*                                                                            */
/* Aufgabe:   uart ausgeben                                                   */
/* Input:     Format uebergeben                                               */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_printf(const char *format, ...);


#undef EXPORT
#endif /* __USART_STM32F10X_H */
/***************************  (C) PRIVATE  2020  *********END OF FILE**********/
