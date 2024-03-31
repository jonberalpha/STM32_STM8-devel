/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2012 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   SvVis3.c                                                      */
/* Autor:       Patrick Weißkirchner                                          */
/* Version:     V1.00                                                         */
/* Date:         21/12/2012                                                    */
/* Description: UART - Library für die Kommunikation zwischen                 */
/*              ARM Minimalsystem und PC-Software "SvVis3 zur Visualisierung  */
/*                                                                            */
/******************************************************************************/
/* History: V1.00  creation                                                    */
/*          V1.1   REJ: Convert to Maier Jakob UART Library (uart_lib.c)      */
/*          V1.2   REJ: Convert to ST-Firmware Library                        */
/******************************************************************************/
#define SVVIS3_MOD
#include <stdio.h>
#include <stdarg.h>
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>
#include <SvVis3.h>

/* -------------------------------- Gloabl Varibales -------------------------*/
uint8_t acqON;

/* ----------------------------Function Prototypes -------------------------*/
static void Puts(char* string);
static void write(void* aData, uint32_t aLenBytes); 
static void read(void* aData, uint32_t aLenBytes);

static char SvVis3_UARTx_get_char(void);     
static void SvVis3_UARTx_put_char(char c);
static void SvVis3_UARTx_put_string(char *string);             
static void SvVis3_UARTx_clear(void);
static void SvVis3_UARTx_saveCursorPos(void);                  
static void SvVis3_UARTx_restoreCursorPos(void);
static void SvVis3_UARTx_setpos(char x,char y);
static void SvVis3_UARTx_printf(const char *format, ...);    


/******************************************************************************/
/*               Initialisiert UART1 für SVis3 Übertragung über               */
/******************************************************************************/
void SvVis3_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  USART_ClockInitTypeDef  USART_ClockInitStructure;
 
  //enable bus clocks 
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  //Set USART1 Tx (PA9) as AF push-pull
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

  //Set USART1 Rx (PA10) as input floating
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

   USART_DeInit(USART1);

  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
 
  USART_ClockInit(USART1, &USART_ClockInitStructure);
 
  USART_InitStructure.USART_BaudRate = 115200;   
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;   
  USART_InitStructure.USART_StopBits = USART_StopBits_1;   
  USART_InitStructure.USART_Parity = USART_Parity_No ;  
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   

  //Write USART1 parameters   
  USART_Init(USART1, &USART_InitStructure);  
  //Enable USART1     
  USART_Cmd(USART1, ENABLE);  

 
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static void Puts(char* string)
{
  while(*string != '\0')  {
    if(*string == '\n')  {
      SvVis3_UARTx_put_char('\r'); // REJ:
      //put_char('\r');
      }
    SvVis3_UARTx_put_char(*string); // REJ:
    //put_char(*string);
    string++;
    }
  SvVis3_UARTx_put_char(0); // REJ:
  //put_char(0);
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static void write(void* aData, uint32_t aLenBytes) 
{
  int i;
    uint8_t* ptr = (uint8_t*)aData;

    for(i=0; i<aLenBytes; i++)   {
      SvVis3_UARTx_put_char(*ptr); // REJ:
      //put_char(*ptr); 
    ptr++;
      }
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static void read(void* aData, uint32_t aLenBytes)
{
  int i;
    uint8_t* ptr = (uint8_t*)aData;

    for(i=0; i<aLenBytes; i++)  {
    *ptr = SvVis3_UARTx_get_char();  // REJ:
      //*ptr=get_char(); 
    ptr++;
      }
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
char SvVis3_Get_Command(void) 
{
  //char cmd = get_char();
   char cmd = SvVis3_UARTx_get_char();  // REJ:

  if(cmd==1)  {
    acqON = SvVis3_UARTx_get_char();  // REJ:
    //acqON = get_char();
    if(acqON)    {
      SvVis3_Message("AcqON");
      }
    else{
      SvVis3_Message("AcqOFF");
      }
    return(0);
  }
  return(cmd);
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void SvVis3_Message(char* aTxt) 
{
  SvVis3_UARTx_put_char(10); // REJ:
    //put_char(10); 
  Puts(aTxt);
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void SvVis3_WriteSV3p13(uint8_t aId, float aData) 
{
    int16_t val = aData;   // hoffentlich funktioniert das auch

  SvVis3_UARTx_put_char(aId); // REJ:
    //put_char(aId); 
    write(&val,2);
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void SvVis3_Write(uint8_t aId, char* aData) 
{
    SvVis3_UARTx_put_char(aId); // REJ:
    //put_char(aId); 
  Puts(aData); 
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void SvVis3_VectHeader(uint8_t aId, uint8_t aNVals)
{ 
  SvVis3_UARTx_put_char(aId); // REJ:
  //put_char(aId); 
  SvVis3_UARTx_put_char(aNVals); // REJ:
  //put_char(aNVals);
}
    
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void SvVis3_Write_I16(uint8_t aId, int16_t aData)
{ 
  SvVis3_UARTx_put_char(aId+10); // REJ:
  //put_char(aId+10); 
  write(&aData,2); 
}
    
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void SvVis3_Write_I32(uint8_t aId, int32_t aData)
{ 
  SvVis3_UARTx_put_char(aId); // REJ:
  //put_char(aId); 
  write(&aData,4); 
}
    
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void SvVis3_WriteSV2(uint8_t aId, float aData)
{ 
  SvVis3_UARTx_put_char(aId+20); // REJ:
  //put_char(aId+20); 
  write(&aData,4); 
}
    
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
int16_t SvVis3_Read_I16()
{ 
int16_t ret; 

  read(&ret,2); 
  return(ret); 
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
int32_t SvVis3_Read_I32()
{ 
int32_t ret; 

  read(&ret,4); 
  return ret; 
}
    
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
float SvVis3_ReadF()
{ 
float ret; 

  read(&ret,4); 
  return ret; 
}    


//---------------------------- internal functions --------------------------------

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static char SvVis3_UARTx_get_char(void)     //liest ein Zeichen von UART1 ein
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    return USART_ReceiveData(USART1);
}



/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static void SvVis3_UARTx_put_char(char c)     //gibt ein Zeichen auf  UART1 aus
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    USART_SendData(USART1,c);
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static void SvVis3_UARTx_put_string(char *string)   //gibt einen Text auf einem der 5 UARTs aus
{
  while (*string)
  {
    SvVis3_UARTx_put_char(*string++);  //solange ein Zeichen im String ist Zeichen ausgeben und Pointer erhöhen  
  }
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static void SvVis3_UARTx_clear()          //löscht den Inhalt der Konsole
{
  SvVis3_UARTx_put_string("\x1b[2J");        //Sendet diese Zeichenfolge
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static void SvVis3_UARTx_saveCursorPos()                    //speichert die derzeitige Cursorposition
{
  SvVis3_UARTx_put_string("\x1b[s");                        //Sendet diese Zeichenfolge
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static void SvVis3_UARTx_restoreCursorPos()                  //geht zur gespeicherten Cursorposition
{
  SvVis3_UARTx_put_string("\x1b[u");                        //Sendet diese Zeichenfolge
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static void SvVis3_UARTx_setpos(char x,char y)                //Setzt den Cursor eines UARTs auf eine bestimmte Position
{
  SvVis3_UARTx_printf("\x1b[%d;%dH",y,x);                      //Sendet die Escape-Sequenz
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static void SvVis3_UARTx_printf(const char *format, ...)            //UART-Printf - ACHTUNG: max. Länge!
{
  static char buffer[1024];                        //Buffer
  va_list  argptr;                            //Argument-Liste
  va_start( argptr, format );
  vsprintf( buffer, format, argptr );                    //Mit sprintf-Funktion in Buffer übertragen
  va_end  ( argptr );
  buffer[1024-1]=0;                            //Zur Sicherheit
  SvVis3_UARTx_put_string(buffer);
}

