/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   USART_STM32F10X.c                                             */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        11.05.2020                                                    */
/* Description: USART Functions                                               */
/*              Header-File stellt defines und Prototypen zur Verfuegung      */
/*              Befehle sind hier ausprogrammiert                             */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define USART_STM32F10X_MOD
#include "USART_STM32F10X.h"

/*____________________________ DEFINES _______________________________________*/
#define ASCII_ESC    27     // \033 or \x1b | CMD Start
#define ASCII_CLRSCR "[2J"  // whole cmd-string: \033[2J; \x1b[2J
#define ASCII_HOME   "[H"
#define ASCII_SVRPOS "[s"
#define ASCII_RESPOS "[u"

/*___________________________ SUBROUTINES ____________________________________*/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_init(unsigned long baud)     */
/*                                                                            */
/* Aufgabe:   USART1 PA9 und PA10 initialisieren                              */
/* Input:     BAUDRATE                                                        */
/* return:                                                                    */
/******************************************************************************/
void uart_init(unsigned long baud)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;            //GPIOA CLK enablen
  GPIOA->CRH &= ~0x0FF0;                         //Konfig-Bits von PA9 und PA10 loeschen
  GPIOA->CRH |= 0x04B0;                         //PA9 als AFOPP und PA10 als IPF
  
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  USART1->BRR = SystemCoreClock/baud;           //SystemCoreClock(z.B.: 8MHz/72MHz)/Baudrate
  USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE); //TxD und RxD enablen
  USART1->CR1 |= USART_CR1_UE;                   //USART1 enablen
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_clear(void)                  */
/*                                                                            */
/* Aufgabe:   Screen clear                                                    */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void uart_clear(void)
{
  // Clear Screen ...
  uart_put_char(ASCII_ESC);
  uart_put_string(ASCII_CLRSCR);
  // ... go to home position
  uart_put_char(ASCII_ESC);
  uart_put_string(ASCII_HOME);
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_put_char(char c)             */
/*                                                                            */
/* Aufgabe:   Zeichen senden                                                  */
/* Input:     char                                                            */
/* return:                                                                    */
/******************************************************************************/
void uart_put_char(char ch)
{
  while(!(USART1->SR & USART_SR_TXE));
  USART1->DR = ch;
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_put_string(char *string)     */
/*                                                                            */
/* Aufgabe:   Zeichenkette/String senden                                      */
/* Input:     string                                                          */
/* return:                                                                    */
/******************************************************************************/
void uart_put_string(char *string)
{
  while(*string)
  {
    uart_put_char(*string++);
  }
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    nib2asc(char *nib)                */
/*                                                                            */
/* Aufgabe:   Function converts a Nibble(0-F) to an ASCII ('0'-'F')           */
/* Input:     nib: Nibble to convert                                          */
/* Output:    nib: Converted Nibble                                           */
/* return:                                                                    */
/******************************************************************************/
void nib2asc(char *nib)
{
  *nib &= 0x0F; // extract Nibble
  if(*nib >= 0x0A) // Hex Value A-F ?
  {
    *nib -= 0x0A;
    *nib += 'A';
  }
  else // Hex value 0 - 9
  {
    *nib += '0';
  }
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_put_hex(char c)              */
/*                                                                            */
/* Aufgabe:   Ausgabe eines 8 Bit Hex Wertes als ASCII String auf UART1       */
/* Input:     char c                                                          */
/* Output:                                                                    */
/* return:                                                                    */
/******************************************************************************/
void uart_put_hex(char ch)
{
  char help;
  help = (ch>>4)&0x0f; // Extract High Nibble
  nib2asc(&help); // Convert to ASCII Code
  uart_put_char(help); // Send High Nibble auf USART1
  help = ch&0x0f; // Extract Low Nibble
  nib2asc(&help); // Convert to ASCII Code
  uart_put_char(help); // Send Low Nibble auf USART1
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_get_char(void)               */
/*                                                                            */
/* Aufgabe:   Zeichen empfangen                                               */
/* Input:                                                                     */
/* return:    char                                                            */
/******************************************************************************/
char uart_get_char(void)
{
  char ch;
  
  while(!(USART1->SR & USART_SR_RXNE));
  ch = USART1->DR;
  return ch;
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_get_string(char *string)     */
/*                                                                            */
/* Aufgabe:   String empfangen                                                */
/* Input:     char *buffer                                                    */
/* return:                                                                    */
/******************************************************************************/
void uart_get_string(char *string)
{
  char ch;
  int cntr = 0;
  
  while(1)
  {
    ch = uart_get_char();
    if(ch == 13) // \n
    {
      string[cntr] = 0;
      return;
    }
    else
    {
      string[cntr] = ch;
      cntr++;
    }
  }
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_saveCursorPos(void)          */
/*                                                                            */
/* Aufgabe:   Cursor Position speichern                                       */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void uart_saveCursorPos(void)         //speichert die derzeitige Cursorposition
{
  uart_put_char(ASCII_ESC);
  uart_put_string(ASCII_SVRPOS);      //Sendet diese Zeichenfolge
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_restoreCursorPos(void)       */
/*                                                                            */
/* Aufgabe:   Cursor Position wiederherstellen                                */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void uart_restoreCursorPos(void)      //geht zur gespeicherten Cursorposition
{
  uart_put_char(ASCII_ESC);
  uart_put_string(ASCII_RESPOS);      //Sendet diese Zeichenfolge
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_setpos(char x, char y)       */
/*                                                                            */
/* Aufgabe:   Cursor Position setzen                                          */
/* Input:     char x, char y                                                  */
/* return:                                                                    */
/******************************************************************************/
void uart_setpos(char x,char y)        //Setzt den Cursor eines UARTs auf eine bestimmte Position
{
  uart_put_char(ASCII_ESC);
  uart_printf("[%d;%dH",y,x);         //Sendet die Escape-Sequenz
}

/******************************************************************************/
/*            U N T E R P R O G R A M M:  uart_printf(const char *format, ...)*/
/*                                                                            */
/* Aufgabe:   uart ausgeben                                                   */
/* Input:     Format uebergeben                                               */
/* return:                                                                    */
/******************************************************************************/
void uart_printf(const char *format, ...)      //UART-Printf - ACHTUNG: max. Laenge!
{
  static char buffer[1024];                    //Buffer
  va_list  argptr;                            //Argument-Liste
  va_start( argptr, format );
  vsprintf( buffer, format, argptr );          //Mit sprintf-Funktion in Buffer UEbertragen
  va_end  ( argptr );
  buffer[1024-1]=0;                            //Zur Sicherheit
  uart_put_string(buffer);
}
