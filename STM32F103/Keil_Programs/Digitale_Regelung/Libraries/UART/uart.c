//
//  (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   
//                                                                            
//   File Name:     uart.c                                                   
//   Autor:       Patrick Weißkirchner                                        
//   Version:     V1.00                                                        
//   Date:       17/04/2013                                                    
//   Description:   UniversalMover-Uart-Library                                          
//
//   History:     V1.00  creation
//

#define UART_MOD

//  Libraries einbinden
#include <uart.h>

void UARTx_put_string(char *string);             
void UARTx_clear(void);
void UARTx_saveCursorPos(void);                  
void UARTx_restoreCursorPos(void);
void UARTx_setpos(char x,char y);
void UARTx_printf(const char *format, ...);    



//  Initialisiert UART3 für UniversalMover
void Usart3Init(void)
{
 
  GPIO_InitTypeDef GPIO_InitStructure;
 
  USART_InitTypeDef USART_InitStructure;
  USART_ClockInitTypeDef  USART_ClockInitStructure;
 
  //enable bus clocks
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);


  //Set USART3 Tx (PC.10) as AF push-pull
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
  //Set USART3 Rx (PC.11) as input floating
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  USART_DeInit(USART3);
 
  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;   //Disable
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
 
  USART_ClockInit(USART3, &USART_ClockInitStructure);
 
  USART_InitStructure.USART_BaudRate = 9600;   
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;   
  USART_InitStructure.USART_StopBits = USART_StopBits_1;   
  USART_InitStructure.USART_Parity = USART_Parity_No ;  
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   


  //Write USART3 parameters   
  USART_Init(USART3, &USART_InitStructure);  
  //Enable USART3     
  USART_Cmd(USART3, ENABLE);
 
}

void UARTx_put_string(char *string)               //gibt einen Text auf einem der 5 UARTs aus
{
  while (*string)
  {
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);        //solange ein Zeichen im String ist
    USART_SendData(USART3,*string++);                    //Zeichen ausgeben und Pointer erhöhen  
  }
}

void UARTx_clear()                        //löscht den Inhalt der Konsole
{
  UARTx_put_string("\x1b[2J");                        //Sendet diese Zeichenfolge
}

void UARTx_saveCursorPos()                    //speichert die derzeitige Cursorposition
{
  UARTx_put_string("\x1b[s");                        //Sendet diese Zeichenfolge
}

void UARTx_restoreCursorPos()                  //geht zur gespeicherten Cursorposition
{
  UARTx_put_string("\x1b[u");                        //Sendet diese Zeichenfolge
}

void UARTx_setpos(char x,char y)                //Setzt den Cursor eines UARTs auf eine bestimmte Position
{
  UARTx_printf("\x1b[%d;%dH",y,x);                      //Sendet die Escape-Sequenz
}

void UARTx_printf(const char *format, ...)            //UART-Printf - ACHTUNG: max. Länge!
{
  static char buffer[1024];                        //Buffer
  va_list  argptr;                            //Argument-Liste
  va_start( argptr, format );
  vsprintf( buffer, format, argptr );                    //Mit sprintf-Funktion in Buffer übertragen
  va_end  ( argptr );
  buffer[1024-1]=0;                            //Zur Sicherheit
  UARTx_put_string(buffer);
}
