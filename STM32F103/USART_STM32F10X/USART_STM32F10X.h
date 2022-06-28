/******************************************************************************/
/* File Name:   USART_STM32F10X.h                                             */
/* Autor: 	    Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        11.05.2020                                                    */
/* Description: USART Functions                                               */
/******************************************************************************/
/* History: 	V1.00  creation                                                 */
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
/*            SUBROUTINE:    uart_init(unsigned long baud)                    */
/*                                                                            */
/* Purpose:   Initialize USART1 PA9 and PA10                                  */
/* Input:     BAUDRATE                                                        */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_init(unsigned long baud);

/******************************************************************************/
/*            SUBROUTINE:    uart_clear(void)                                 */
/*                                                                            */
/* Purpose:   Clear screen                                                    */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_clear(void);

/******************************************************************************/
/*            SUBROUTINE:    uart_put_char(char c)                            */
/*                                                                            */
/* Purpose:   Send char                                                       */
/* Input:     char                                                            */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_put_char(char ch);

/******************************************************************************/
/*            SUBROUTINE:    uart_put_string(char *string)                    */
/*                                                                            */
/* Purpose:   Send string                                                     */
/* Input:     string                                                          */
/* return:                                                                    */
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
/* Purpose:   Ouptut 8 bit hex value as ASCII string over UART1               */
/* Input:     char c                                                          */
/* Output:                                                                    */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_put_hex(char ch);

/******************************************************************************/
/*            SUBROUTINE:    uart_get_char(void)                              */
/*                                                                            */
/* Purpose:   Receive char                                                    */
/* Input:                                                                     */
/* return:    char                                                            */
/******************************************************************************/
EXPORT char uart_get_char(void);

/******************************************************************************/
/*            SUBROUTINE:    uart_get_string(char *string)                    */
/*                                                                            */
/* Purpose:   Receive string                                                  */
/* Input:     char *buffer                                                    */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_get_string(char *string);

/******************************************************************************/
/*            SUBROUTINE:    uart_saveCursorPos(void)                         */
/*                                                                            */
/* Purpose:   Save cursor position                                            */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_saveCursorPos(void);

/******************************************************************************/
/*            SUBROUTINE:    uart_restoreCursorPos(void)                      */
/*                                                                            */
/* Purpose:   Restore cursor position                                         */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_restoreCursorPos(void);

/******************************************************************************/
/*            SUBROUTINE:    uart_setpos(char x, char y)                      */
/*                                                                            */
/* Purpose:   Set cursor position                                             */
/* Input:     char x, char y                                                  */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_setpos(char x,char y);

/******************************************************************************/
/*            SUBROUTINE:  uart_printf(const char *format, ...)               */
/*                                                                            */
/* Purpose:   Print formated string over uart                                 */
/* Input:     Format-string                                                   */
/* return:                                                                    */
/******************************************************************************/
EXPORT void uart_printf(const char *format, ...);


#undef EXPORT
#endif /* __USART_STM32F10X_H */
