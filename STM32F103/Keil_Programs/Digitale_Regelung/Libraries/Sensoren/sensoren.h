/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   Sensor.h                                                      */
/* Autor:       Josef Reisinger                                               */
/* Version:     V1.00                                                         */
/* Date:        07/0    2/2016                                                */
/* Description: Einlesen der Sensorwerte für Digitalen Regler                 */
/*              (Istwert, Sollwert) mittels ADC und DMA Controller            */  
/******************************************************************************/
/* History:     V1.00  creation                                               */
/******************************************************************************/
/*  Sensors:                                                                  */
/*  PB1     -> ADC1_IN9  Analog Input for desired Value (Sollwert)            */
/*  PC4     -> ADC1_IN14 Analog Input for measured Value (Istwert)            */
/******************************************************************************/

/* --------------------Define to prevent recursive inclusion -----------------*/
#ifndef __SENSOREN_H
#define __SENSOREN_H

#include <stm32f10x.h>

/* ------------------------------Includes -------------------------------------*/
#ifdef SENSOREN_MOD
#define EXPORT
#else
#define EXPORT extern
#endif 

/*  ----------------------- Sensorfunktionen -----------------------------*/
EXPORT void     SensorInit(void);
EXPORT void     DMA_Config(void);
EXPORT int16_t  Get_Desired_Value(void);
EXPORT int16_t  Get_Measured_Value(void);

// ZOG: added
EXPORT int16_t Get_ADC_PB1(void);
EXPORT int16_t Get_ADC_PC4(void);
EXPORT int16_t Get_ADC_PA0(void);
EXPORT int16_t Get_ADC_PA1(void);
EXPORT int16_t Get_ADC_PA2(void);
EXPORT int16_t Get_ADC_PA3(void);

#undef  EXPORT
#endif 

