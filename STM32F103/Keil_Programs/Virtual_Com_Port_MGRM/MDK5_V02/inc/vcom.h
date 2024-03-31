/******************** (C) COPYRIGHT 2010 Bernhard Müllner **********************
* File Name          : vcom.h
* Author             : Bernhard Müllner
* Version            : V1.0.0
* Date               : 04/08/2010
* Description        : overwrite printf for virtuell Comport
********************************************************************************/
#ifndef __VCOM_H
#define __VCOM_H

//___ I N C L U D E S __________________________________________________________
#include "usb_mem.h"
#include "usb_regs.h"
#include "usb_desc.h"
#include <stdio.h>

//___ P R O T O T Y P E S ______________________________________________________

char GetChar (void);
int SendChar(char c);
u16 GetEPTxCTR(u8 bEpNum);

//___ M A C R O S ______________________________________________________________

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define _GetEPTxCTR(bEpNum) ((u16)_GetENDPOINT(bEpNum) & EP_CTR_TX)

//___ E X T E R N A L ___ D E F I N I T I O S __________________________________


extern FILE __stdout;
extern FILE __stdin;

//___ G L O B A L ___ V A R I A B L E S ________________________________________

extern u32 count_out;
extern u32 count_in;
extern u8 buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];
extern int rx_flag,tx_flag;
extern int break_flag;

#endif /* __VCOM_H */
