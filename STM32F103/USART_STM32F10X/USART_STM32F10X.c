/******************************************************************************/
/* File Name:   USART_STM32F10X.c                                             */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        11.05.2020                                                    */
/* Description: USART Functions                                               */
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
/*            SUBROUTINE:    uart_init(unsigned long baud)                    */
/*                                                                            */
/* Purpose:   Initialize USART1 PA9 and PA10                                  */
/* Input:     BAUDRATE                                                        */
/* return:                                                                    */
/******************************************************************************/
void uart_init(unsigned long baud)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;           //Enable GPIOA CLK
  GPIOA->CRH &= ~0x0FF0;                        //Delete Config-Bits of PA9 and PA10
  GPIOA->CRH |= 0x04B0;                         //PA9 as AFOPP and PA10 as IPF

  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  USART1->BRR = SystemCoreClock/baud;           //SystemCoreClock(e.g.: 8MHz/72MHz)/Baudrate
  USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE); //Enable TxD and RxD
  USART1->CR1 |= USART_CR1_UE;                  //Enable USART1
}

/******************************************************************************/
/*            SUBROUTINE:    uart_clear(void)                                 */
/*                                                                            */
/* Purpose:   Clear screen                                                    */
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
/*            SUBROUTINE:    uart_put_char(char c)                            */
/*                                                                            */
/* Purpose:   Send char                                                       */
/* Input:     char                                                            */
/* return:                                                                    */
/******************************************************************************/
void uart_put_char(char ch)
{
  while(!(USART1->SR & USART_SR_TXE));
  USART1->DR = ch;
}

/******************************************************************************/
/*            SUBROUTINE:    uart_put_string(char *string)                    */
/*                                                                            */
/* Purpose:   Send string                                                     */
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
/*            SUBROUTINE:    nib2asc(char *nib)                               */
/*                                                                            */
/* Purpose:   Function converts a Nibble(0-F) to an ASCII ('0'-'F')           */
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
/*            SUBROUTINE:    uart_put_hex(char c)                             */
/*                                                                            */
/* Purpose:   Ouptut 8 bit hex value as ASCII string over UART1               */
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
/*            SUBROUTINE:    uart_get_char(void)                              */
/*                                                                            */
/* Purpose:   Receive char                                                    */
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
/*            SUBROUTINE:    uart_get_string(char *string)                    */
/*                                                                            */
/* Purpose:   Receive string                                                  */
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
/*            SUBROUTINE:    uart_saveCursorPos(void)                         */
/*                                                                            */
/* Purpose:   Save cursor position                                            */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void uart_saveCursorPos(void)
{
  uart_put_char(ASCII_ESC);
  uart_put_string(ASCII_SVRPOS);
}

/******************************************************************************/
/*            SUBROUTINE:    uart_restoreCursorPos(void)                      */
/*                                                                            */
/* Purpose:   Restore cursor position                                         */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void uart_restoreCursorPos(void)
{
  uart_put_char(ASCII_ESC);
  uart_put_string(ASCII_RESPOS);
}

/******************************************************************************/
/*            SUBROUTINE:    uart_setpos(char x, char y)                      */
/*                                                                            */
/* Purpose:   Set cursor position                                             */
/* Input:     char x, char y                                                  */
/* return:                                                                    */
/******************************************************************************/
void uart_setpos(char x,char y)
{
  uart_put_char(ASCII_ESC);
  uart_printf("[%d;%dH",y,x);
}

/******************************************************************************/
/*            SUBROUTINE:  uart_printf(const char *format, ...)               */
/*                                                                            */
/* Purpose:   Print formated string over uart                                 */
/* Input:     Format-string                                                   */
/* return:                                                                    */
/******************************************************************************/
void uart_printf(const char *format, ...)
{
  static char buffer[1024];
  va_list  argptr;
  va_start( argptr, format );
  vsprintf( buffer, format, argptr );
  va_end  ( argptr );
  buffer[1024-1]=0;
  uart_put_string(buffer);
}
