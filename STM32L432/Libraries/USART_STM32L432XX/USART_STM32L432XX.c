/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   USART_STM32L432XX.c                                           */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        28.09.2021                                                    */
/* Description: USART_STM32L432XX Library                                     */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define USART_STM32L432XX_MOD

/*_______________________________ INCLUDES ___________________________________*/
#include "USART_STM32L432XX.h"

/*_____________________________ UNTERPROGRAMME _______________________________*/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    uart_init(unsigned long baud)     */
/*                                                                            */
/* Aufgabe:   USART1 PA9 und PA10 initialisieren                              */
/* Input:     BAUDRATE                                                        */
/* return:                                                                    */
/******************************************************************************/
void uart_init(unsigned long baud)
{
  //PA9 out TX/PA10 in RX
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;          //GPIOA CLK enablen
  GPIOA->MODER &= ~0x3C0000;                     //Konfig-Bits von PA9 und PA10 loeschen
  GPIOA->MODER |= 0x280000;                     //PA9 und PA10 als AF
  
  GPIOA->OSPEEDR &= ~0x3C0000;                  //Speed loeschen
  GPIOA->OSPEEDR |= 0x3C0000;                    //Highest speed
  
  GPIOA->AFR[1] |= 0x770;                        //Alternate function: AF7 (111) for USART1
  
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;          //enable UART clock
  USART1->BRR = SystemCoreClock/baud;           //SystemCoreClock(z.B.: 8MHz/72MHz)/Baudrate
  USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE); //TxD und RxD enablen
  USART1->CR2 &= ~USART_CR2_STOP;                //1 Stop bit
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
  while(!(USART1->ISR & USART_ISR_TXE));
  USART1->TDR = ch;
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
  
  while(!(USART1->ISR & USART_ISR_RXNE));
  ch = USART1->RDR;
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
  va_list argptr;                             //Argument-Liste
  va_start( argptr, format );
  vsprintf( buffer, format, argptr );          //Mit sprintf-Funktion in Buffer UEbertragen
  va_end  ( argptr );
  buffer[1024-1]=0;                            //Zur Sicherheit
  uart_put_string(buffer);
}