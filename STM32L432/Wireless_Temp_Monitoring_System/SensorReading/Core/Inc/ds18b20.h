/**
 *******************************************************************************
 * @file    ds18b20.h
 * @authors Berger, Ibrahim, Markovic, Meyerowitsch
 * @brief   This file provides the public readtemp()-prototype for the
 *          ds18b20.c file
 ******************************************************************************
 */

#ifndef	_DS18B20_H
#define	_DS18B20_H

#define DS18B20_PORT GPIOB
#define DS18B20_PIN GPIO_PIN_0

// exported protoype
float readtemp(void);

#endif // _DS18B20_H

