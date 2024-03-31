/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   I2C_STM32F10X.h                                               */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        27.08.2020                                                    */
/* Description: I2C Library                                                   */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef I2C_STM32F10X_H_INCLUDED
#define I2C_STM32F10X_H_INCLUDED

/*_______________________________ INCLUDES ___________________________________*/
#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>

#ifdef  I2C_STM32F10X_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/*___________________________ EXPORTED FUNCTIONS ____________________________*/
/******************************************************************************/
/*            U N T E R P R O G R A M M:    i2c_init()                        */
/*                                                                            */
/* Aufgabe:   Initialize the I2C interface                                    */
/*                                                                            */
/*            @param registerStruct May be either I2C1 (SCL=PB6, SDA=PB7) or  */
/*                   I2C2 (SCL=PB10, SDA=PB11)                                */
/*            @param remap Whether to remap I2C1 to the alternative pins      */
/*                   (SCL=PB8, SDA=PB9)                                       */
/*            @param fastMode false=100 kHz, true=400 kHz                     */
/*            @param apb1_clock clock frequency of APB1 peripherals           */
/*                                                                            */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void i2c_init(I2C_TypeDef* registerStruct, bool remap, bool fastMode,  
            uint32_t apb1_clock);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    i2c_communicate()                 */
/*                                                                            */
/* Aufgabe:   Perform an I²C transaction, which sends 0 or more data bytes,   */
/*            followed by receiving 0 or more data bytes.                     */
/*                                                                            */
/*            @param registerStruct May be either I2C1 or I2C2                */
/*            @param slave_addr 7bit slave_addr (will be shifted              */
/*                   within this function)                                    */
/*            @param send_buffer Points to the buffer that contains           */
/*                   the data bytes that shall be sent (may be 0 if not used) */
/*            @param send_size Number of bytes to send                        */
/*            @param receive_buffer Points to the buffer that will be filled  */
/*                   with the received bytes (may be 0 if not used)           */
/*            @param receive_size Number of bytes to receive                  */
/*            @return Number of received data bytes, or -1 if sending failed  */
/*                                                                            */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT int i2c_communicate(I2C_TypeDef* registerStruct, uint8_t slave_addr, 
    uint8_t* send_buffer, int send_size, uint8_t* receive_buffer, int receive_size);

#undef EXPORT
#endif /* __I2C_STM32F10X_H */
/***************************  (C) PRIVATE  2020  *********END OF FILE*********/
