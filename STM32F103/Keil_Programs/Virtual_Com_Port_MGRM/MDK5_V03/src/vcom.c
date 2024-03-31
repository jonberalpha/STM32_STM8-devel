/******************** (C) COPYRIGHT 2010 Bernhard Müllner **********************
* File Name          : vcom.c
* Author             : Bernhard Müllner
* Version        
    : V1.0.0
* Date               : 04/08/2010
* Description        : overwrite printf for virtuell Comport
********************************************************************************/

//___ I N C L U D E S __________________________________________________________
#include "vcom.h"
#include "usb_regs.h"


//___ G L O B A L ______________________________________________________________

struct __FILE { int handle; };
FILE __stdout;
FILE __stdin;
int break_flag;




char GetChar (void)  
{
	rx_flag = 0;
	
	while(rx_flag == 0)			 	
		SetEPRxValid(ENDP3);
	rx_flag = 0;
	
	return buffer_out[0];
}   


int SendChar(char c)
{
	int in = 1;
	char zeichen[1];
	int index = 0;

    if (c == '\n')
	{ 
		SendChar('\r');
	}

	zeichen[0] = c;

	UserToPMABufferCopy( (u8 *)zeichen , ENDP1_TXADDR, in);
	SetEPTxCount(ENDP1, in);
	SetEPTxValid(ENDP1);

	for( index=0;index<=30000;index++);	 //should be a if(Endpoint is free)
   
    return 0;
}

PUTCHAR_PROTOTYPE
{
  SendChar((char) ch);
  return ch;
} 



