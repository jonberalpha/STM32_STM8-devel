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
#ifndef __SVVIS3_H
#define __SVVIS3_H

#include <stm32f10x.h>

/* ----------------------- Global Definitions -----------------------------*/
extern uint8_t acqON;

#ifdef SVVIS3_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/* ------------------------ Exported Funtions ---------------------------*/
EXPORT void   SvVis3_Init(void);
EXPORT char   SvVis3_Get_Command(void);
EXPORT void   SvVis3_Message(char* aTxt);
EXPORT void   SvVis3_Write_p13(uint8_t aId, float aData) ;  
EXPORT void   SvVis3_Write(uint8_t aId, char* aData);    // Send 8 Bit data
EXPORT void   SvVis3_VectHeader(uint8_t aId, uint8_t aNVals);
EXPORT void   SvVis3_Write_I16(uint8_t aId, int16_t aData);  // Send 16 Bit data
EXPORT void   SvVis3_Write_I32(uint8_t aId, int32_t aData);  // Send 32 Bit data
EXPORT void   SvVis3_WriteSV2(uint8_t aId, float aData);
EXPORT int16_t   SvVis3_Read_I16(void);
EXPORT int32_t   SvVis3_Read_I32(void);
EXPORT float   SvVis3_ReadF(void);

#undef  EXPORT

#endif



