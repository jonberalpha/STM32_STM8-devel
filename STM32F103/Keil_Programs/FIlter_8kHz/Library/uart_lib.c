/********************************************************************************************************************************/
/* \ *\     U A R T - L I B R A R Y                                          16.04.2011                                    \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    HTL Hollabrunn   AUSTRIA                                                                                        \* / */
/* / *\    Author:          Maier Jakob                                                                                    \* \ */
/* \ *\    Date:            April 2011                                                                                     \* / */
/* / *\    Version:         1.0                                                                                            \* \ */
/* \ *\    Description:     Beinhaltet alle wesentlichen Unterprogramme die zur Kommunikation mithilfe des UARTs           \* / */
/* / *\                     notwendig sind                                                                                 \* \ */
/* \ *\    (c) Maier Jakob                                                                                                 \* / */
/********************************************************************************************************************************/
#define __UART_LIB_MOD
#include "uart_lib.h"
#include "gpio_lib.h"

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void init_UARTx(USART_TypeDef* UARTx,unsigned long baudrate,char databits,char stop,char parity)//Initialisiert einen der 5 UARTs
{
  switch((int)UARTx)                                //Ports initialisieren (inkl. Remap)
  {  case (int)USART1:  init_port(GPIOA, 9, 9,AFO_pushpull,frequency_50MHz);  //PA9=Tx
              init_port(GPIOA,10,10,INPUT_floating,0);        //PA10=Rx
              RCC->APB2ENR |= RCC_APB2ENR_USART1EN;          //USART1 mit einem Takt versrogen
              break;
    case (int)USART2:  init_port(GPIOA, 2, 2,AFO_pushpull,frequency_50MHz);  //PA2=Tx
              init_port(GPIOA, 3, 3,INPUT_floating,0);        //PA3=Rx
              RCC->APB1ENR |= RCC_APB1ENR_USART2EN;          //USART2 mit einem Takt versrogen
              break;
    case (int)USART3:  init_port(GPIOD, 8, 8,AFO_pushpull,frequency_50MHz);  //PD8=Tx
              init_port(GPIOD, 9, 9,INPUT_floating,0);        //PD9=Rx
              RCC->APB1ENR |= RCC_APB1ENR_USART3EN;          //USART3 mit einem Takt versrogen
              RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;            //AFIO Peripherie versorgen
              AFIO->MAPR |= 0x30;                    //USART3 Full Remap
              break;
    case (int)UART4:  init_port(GPIOC,10,10,AFO_pushpull,frequency_50MHz);  //PC10=Tx
              init_port(GPIOC,11,11,INPUT_floating,0);        //PC11=Rx
              RCC->APB1ENR |= RCC_APB1ENR_UART4EN;          //UART4 mit einem Takt versrogen
              break;
    case (int)UART5:  init_port(GPIOC,12,12,AFO_pushpull,frequency_50MHz);  //PC12=Tx
              init_port(GPIOD, 2, 2,INPUT_floating,0);        //PD2=Rx
              RCC->APB1ENR |= RCC_APB1ENR_UART5EN;          //UART5 mit einem Takt versrogen
  }
  UARTx->BRR = 8000000L/baudrate;                         //Baudrate festlegen
  UARTx->CR1 |= (USART_CR1_RE | USART_CR1_TE |( (databits==8)?0:USART_CR1_M ));    //aktiviere RX, TX, setze anzahl der Datenbits
     if(parity!=NO_PARITY)                              //Parity setzen
  {  UARTx->CR1 |= ( USART_CR1_PCE | ((parity==ODD_PARITY)?USART_CR1_PS:0) );    //Odd/Even
  }
  if(stop != STOP_1)                                //Stopbits setzen (0,5 und 1,5 Stopbits nicht bei UART4 und UART5 möglich)
  {  UARTx->CR2 |= ( ((stop==STOP_0_5||stop==STOP_1_5)?USART_CR2_STOP_0:0) | ((stop==STOP_2||stop==STOP_1_5)?USART_CR2_STOP_1:0) );
  }
    UARTx->CR1 |= USART_CR1_UE;                                      //aktiviere USART

}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
char UARTx_get_char(USART_TypeDef* UARTx)                      //Empfängt ein Zeichen über einen der 5 UARTs
{
  while (!(UARTx->SR & USART_SR_RXNE));                        //warten, bis Daten empfangen wurden
    return ((char)(UARTx->DR & 0xFF));                        //byteweise lesen
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
char UARTx_received(USART_TypeDef* UARTx)                      //Kontrolliert, ob bei einem der 5 UARTs ein Zeichen empfangen wurde
{
  return (((UARTx->SR & USART_SR_RXNE)!=0)?1:0);                  //Wert zurückgeben (true/false)
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void UARTx_put_char(USART_TypeDef* UARTx,char zeichen)                //Sendet ein Zeichen über einen der 5 UARTs
{
  while (!(UARTx->SR & USART_SR_TXE));                      //warten, bis die letzten Daten gesendet wurden
  UARTx->DR = zeichen;                              //Daten in Senderegister schreiben
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void UARTx_put_hex(USART_TypeDef* UARTx,char zeichen)                //Eine Hex-Zahl über einen der 5 UARTs ausgeben (2 Zeichen)
{
  UARTx_put_char (UARTx,((zeichen >> 4)>=0x0A)?((zeichen >> 4)-0x0A+'A'):((zeichen >> 4)+'0'));  //linkes Nibbel ausgeben
  UARTx_put_char (UARTx,((zeichen&0x0F)>=0x0A)?((zeichen&0x0F)-0x0A+'A'):((zeichen&0x0F)+'0'));  //rechtes Nibbel ausgeben
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void UARTx_put_string(USART_TypeDef* UARTx,char *string)               //Gibt einen Text auf einem der 5 UARTS aus
{
    while (*string)                                  //solange ein Zeichen im String ist
    UARTx_put_char(UARTx,*string++);                      //Zeichen ausgeben und Pointer erhöhen
}

#ifndef PRINTF_BUFFER                                //Es kann eine andere Länge definiert werden
  #define PRINTF_BUFFER 1024                            //UART-Sende Buffer: max. Größe
#endif
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void UARTx_printf(USART_TypeDef* UARTx,const char *format, ...)            //UART-Printf - ACHTUNG: max. Länge!
{
  static char buffer[PRINTF_BUFFER];                        //Buffer
  va_list  argptr;                                //Argument-Liste
  va_start( argptr, format );
  vsprintf( buffer, format, argptr );                        //Mit sprintf-Funktion in Buffer übertragen
  va_end  ( argptr );
  buffer[PRINTF_BUFFER-1]=0;                            //Zur Sicherheit
  UARTx_put_string(UARTx,buffer);
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void UARTx_put_bin(USART_TypeDef* UARTx,char wert)                  //Gibt eine 8bit-Zahl binär aus
{
  UARTx_printf(UARTx,"%d%d%d%d%d%d%d%d",wert&0x80?1:0,wert&0x40?1:0,wert&0x20?1:0,wert&0x10?1:0,wert&0x08?1:0,wert&0x04?1:0,wert&0x02?1:0,wert&0x01?1:0);
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void UARTx_clear(USART_TypeDef* UARTx)                        //löscht den Inhalt der Konsole
{
  UARTx_put_string(UARTx,"\x1b[2J");                        //Sendet diese Zeichenfolge
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void UARTx_saveCursorPos(USART_TypeDef* UARTx)                    //speichert die derzeitige Cursorposition
{
  UARTx_put_string(UARTx,"\x1b[s");                        //Sendet diese Zeichenfolge
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void UARTx_restoreCursorPos(USART_TypeDef* UARTx)                  //geht zur gespeicherten Cursorposition
{
  UARTx_put_string(UARTx,"\x1b[u");                        //Sendet diese Zeichenfolge
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void UARTx_setpos(USART_TypeDef* UARTx,char x,char y)                //Setzt den Cursor eines UARTs auf eine bestimmte Position
{
  UARTx_printf(UARTx,"\x1b[%d;%dH",y,x);                      //Sendet die Escape-Sequenz
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void UARTx_eraseLine(USART_TypeDef* UARTx)                      //löscht die Zeile ab inkl. der Cursorposition
{
  UARTx_put_string(UARTx,"\x1b[K");                        //Sendet diese Zeichenfolge
}


/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void UARTx_style(USART_TypeDef* UARTx,char effekt)                  //Ändert Text- und Hintergrundfarbe sowie Effekte
{
  UARTx_printf(UARTx,"\x1b[%02xm",effekt);                    //Sendet die Escape-Sequenz
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void UARTx_STDstyle(USART_TypeDef* UARTx)                      //Löscht alle Effekte wenn Hintergrund=weiß + Textfarbe=schwarz
{
  UARTx_put_string(UARTx,"\x1b[0;47;30m");                    //Sendet die Escape-Sequenz
}


