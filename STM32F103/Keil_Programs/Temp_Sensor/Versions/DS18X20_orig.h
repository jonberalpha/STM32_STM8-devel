/******************************************************************************/
/*   (C) Copyright Private  2009-2018 All rights reserved. AUSTRIA            */ 
/*                                                                            */ 
/* File Name:   DS18X20.c                                                     */
/* Autor: 	    Berger Jonas                                                  */
/* Version: 	  V1.00                                                         */
/* Date: 		    08.06.2019                                                    */
/* Description: Header-File                                                   */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/

#ifndef DS18X20_H_INCLUDED
#define DS18X20_H_INCLUDED

/*________________________________ DEFINES ___________________________________*/
//Define DS18S20 Command Set
#define ConvertTemp     0x44
#define ReadScratchpad  0xBE
#define WriteScratchpad 0x4E
#define CopyScratchpad  0x48
#define RecallE2        0xB8
#define ReadPowerSupply 0xB4

/*______________________________ BITBANDING __________________________________*/
//Calc peripheral address GPIOB ODR
#define GPIOB_ODR GPIOB_BASE + 3*sizeof(uint32_t)
//Calc peripheral address GPIOA IDR
#define GPIOB_IDR GPIOB_BASE + 2*sizeof(uint32_t) 
//Calc Bit Band Adress from peripheral address: a = peripheral address b = Bit number:
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))
//BB fuer PB1
#define DQ_o *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,1)))  //DQ/PB1
#define DQ_i  *((volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,1))) //DQ/PB1


/*_______________________________ INCLUDES ___________________________________*/
#include <stm32f10x.h>
#include "WAIT_STM32F10X.h"

/*________________________________ EXTERN ____________________________________*/
extern uint8_t datareg[9];

/*______________________________ PROTOTYPES __________________________________*/
void init_ports(void);
void setDQasIPF(void);
void setDQasGPOOD(void);
uint8_t sendMRI(void);
uint8_t readBit(void);
uint8_t readByte(void);
void writeBit(uint8_t val);
void writeByte(uint8_t val);
void skipROM(void);
float convTemp(uint8_t val[]);

#endif /* __DS18X20_H */
