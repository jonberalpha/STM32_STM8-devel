/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   SENS_DS18X20.c                                                */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        15.06.2020                                                    */
/* Description: Liefert Temperaturwert des am Minimalsystem verbauten Sensors */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef SENS_DS18X20_H_INCLUDED
#define SENS_DS18X20_H_INCLUDED

/*_______________________________ INCLUDES ___________________________________*/
#include <stm32f10x.h>
#include "WAIT_STM32F10X.h"

#ifdef  SENS_DS18X20_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/*________________________________ DEFINES ___________________________________*/
//Define DS18X20 Command Set
#define SKIP_ROM          0xCC
#define CONVERT_TEMP      0x44
#define READ_SCRATCHPAD   0xBE
#define WRITE_SCRATCHPAD  0x4E
#define COPY_SCRATCHPAD   0x48
#define RECALL_E2         0xB8
#define READ_POWER_SUPPLY 0xB4

/*______________________________ BITBANDING __________________________________*/
//Calc peripheral address GPIOB ODR
#define GPIOB_ODR GPIOB_BASE + 3*sizeof(uint32_t)
//Calc peripheral address GPIOA IDR
#define GPIOB_IDR GPIOB_BASE + 2*sizeof(uint32_t) 
//Calc Bit Band Adress from peripheral address: a = peripheral address b = Bit number:
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))
//BB fuer PB1
#define DQ_o *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,0))) //DQ out/PB0
#define DQ_i *((volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,0))) //DQ in/PB0

/*____________________________ EXPORTED FUNCTIONS ____________________________*/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    DS18X20_init()                    */
/*                                                                            */
/* Aufgabe:   initializes DS18X20                                             */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void DS18X20_init(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    get_temperature()                 */
/*                                                                            */
/* Aufgabe:   get_temperature                                                 */
/* Input:                                                                     */
/* return:    float temp                                                      */
/******************************************************************************/
EXPORT float get_temperature(void);

#undef EXPORT
#endif /* __SENS_DS18X20_H */
/***************************  (C) PRIVATE  2020  *********END OF FILE**********/
