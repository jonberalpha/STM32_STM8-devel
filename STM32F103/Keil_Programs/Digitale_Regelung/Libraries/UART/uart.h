//
//  (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   
//                                                                            
//   File Name:   uart.h                                                   
//   Autor:       Patrick Weißkirchner                                        
//   Version:     V1.00                                                        
//   Date:        17/04/2013                                                    
//   Description: UniversalMover-Uart-Library                                          
//
//   History:     V1.00  creation
//

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>

#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>



#ifdef UART_MOD
#define EXPORT
#else
#define EXPORT extern
#endif 

//  Initialisiert UART3 für UniversalMover
EXPORT void Usart3Init(void);
EXPORT void UARTx_put_string(char *string);             
EXPORT void UARTx_clear(void);
EXPORT void UARTx_saveCursorPos(void);                  
EXPORT void UARTx_restoreCursorPos(void);
EXPORT void UARTx_setpos(char x,char y);
EXPORT void UARTx_printf(const char *format, ...);  


#undef  EXPORT
#endif 

