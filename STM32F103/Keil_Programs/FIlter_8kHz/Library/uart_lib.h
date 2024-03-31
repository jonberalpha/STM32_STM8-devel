#ifndef  __UART_LIB_H     //keine Mehrfach definition
#define  __UART_LIB_H

#include "stm32f10x.h"
/********************************************************************************************************************************/
/* \ *\     U A R T - L I B R A R Y                                          16.04.2011                                    \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    HTL Hollabrunn   AUSTRIA                                                                                        \* / */
/* / *\    Author:          Maier Jakob                                                                                    \* \ */
/* \ *\    Date:            April 2011                                                                                     \* / */
/* / *\    Version:         1.0                                                                                            \* \ */
/* \ *\    Description:  Beinhaltet alle wesentlichen Unterprogramme die zur Kommunikation mithilfe des UARTs              \* / */
/* / *\                  notwendig sind                                                                                    \* \ */
/* \ *\    (c) Maier Jakob                                                                                                 \* / */
/********************************************************************************************************************************/

//Versionshinweise:
//*****************
//
//Ver. 1.0:  akt. Version


#include <stdarg.h>                                  //va_list (Für UARTx_printf())

//Zu einfachereren Programmierung: falsche Parameter definieren
#define UART1   USART1
#define UART2   USART2
#define UART3   USART3
#define USART4   UART4
#define USART5   UART5


#define NO_PARITY   0x00  //Es wird kein Parity-Bit verwendet
#define ODD_PARITY  0x01  //Ungerade Bitanzahl
#define EVEN_PARITY  0x02    //Gerade Bitanzahl

#define STOP_0_5  0x03  //0,5 Stopbits    (nicht unter UART4 und UART5)
#define STOP_1    0x01  //1 Stopbit
#define STOP_1_5  0x04  //1,5 Stopbits    (nicht unter UART4 und UART5)
#define STOP_2    0x02  //2 Stopbits


/* ------------------------UART-KONSOLENEFFEKTE ------------------------*/
#define EFFEKT_NONE      0x00
#define EFFEKT_BRIGHT    0x01
#define EFFEKT_DIM      0x02
#define EFFEKT_UNDERSCORE  0x04
#define EFFEKT_BLINK    0x05
#define EFFEKT_REVERSE    0x07
#define EFFEKT_HIDDEN    0x08

#define FOREGROUND_BLACK  0x30
#define FOREGROUND_RED    0x31
#define FOREGROUND_GREEN  0x32
#define FOREGROUND_YELLOW  0x33
#define FOREGROUND_BLUE    0x34
#define FOREGROUND_MAGENTA  0x35
#define FOREGROUND_CYAN    0x36
#define FOREGROUND_WHITE  0x37

#define BACKGROUND_BLACK  0x40
#define BACKGROUND_RED    0x41
#define BACKGROUND_GREEN  0x42
#define BACKGROUND_YELLOW  0x43
#define BACKGROUND_BLUE    0x44
#define BACKGROUND_MAGENTA  0x45
#define BACKGROUND_CYAN    0x46
#define BACKGROUND_WHITE  0x47

/* ------------------Vordefinierte Designes für printf-Funktion: -----------------------------*/
#define TXT_BLA  "\x1b[0;47;30m"    //Schwarzer Text
#define TXT_RED  "\x1b[0;47;31;1m"    //Roter Text
#define TXT_GRE  "\x1b[0;47;32m"    //Dunkelgrüner Text
#define TXT_YEL  "\x1b[0;47;33;1m"    //Gelber Text
#define TXT_BLU  "\x1b[0;47;34;1m"    //Blauer Text
#define TXT_MAG  "\x1b[0;47;35;1m"    //Rosa Text
#define TXT_CYA  "\x1b[0;47;36;1m"    //Türkiser Text
#define TXT_WHI  "\x1b[0;47;37;1m"    //Weißer Text
#define TXT_UE1  "\x1b[0;47;31;04m"    //Überschrift 1 (Dunkelrot + Unterstrichen)
#define TXT_UE2  "\x1b[0;47;30;43m"    //Überschrift 2 (Schwarz + Dunkelgelb hinterlegt)
#define TXT_BLR  "\x1b[0;47;31;1;5m"  //Roter Blinktext

#ifdef __UART_LIB_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/****************************************************************************/
/*                                                */
/****************************************************************************/ 
EXPORT void init_UARTx(USART_TypeDef* UARTx,unsigned long baudrate,char databits,char stop,char parity);
EXPORT char UARTx_get_char(USART_TypeDef* UARTx);
EXPORT char UARTx_received(USART_TypeDef* UARTx);
EXPORT void UARTx_put_char(USART_TypeDef* UARTx,char zeichen);
EXPORT void UARTx_put_hex(USART_TypeDef* UARTx,char zeichen);
EXPORT void UARTx_put_string(USART_TypeDef* UARTx,char *string);
EXPORT void UARTx_printf(USART_TypeDef* UARTx,const char *format, ...);
EXPORT void UARTx_put_bin(USART_TypeDef* UARTx,char wert);
EXPORT void UARTx_clear(USART_TypeDef* UARTx);
EXPORT void UARTx_saveCursorPos(USART_TypeDef* UARTx);
EXPORT void UARTx_restoreCursorPos(USART_TypeDef* UARTx);
EXPORT void UARTx_setpos(USART_TypeDef* UARTx,char x,char y);
EXPORT void UARTx_eraseLine(USART_TypeDef* UARTx);
EXPORT void UARTx_style(USART_TypeDef* UARTx,char effekt);
EXPORT void UARTx_STDstyle(USART_TypeDef* UARTx);

#undef  EXPORT
#endif
