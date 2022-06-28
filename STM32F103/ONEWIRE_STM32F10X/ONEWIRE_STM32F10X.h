/******************************************************************************/
/* File Name:   ONEWIRE_STM32F10X.h                                           */
/* Autor: 	    Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        10.07.2020                                                    */
/* Description: 1-wire Library                                                */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef ONEWIRE_STM32F10X_H_INCLUDED
#define ONEWIRE_STM32F10X_H_INCLUDED

/*_______________________________ INCLUDES ___________________________________*/
#include <stm32f10x.h>
#include "WAIT_STM32F10X.h"

#ifdef  ONEWIRE_STM32F10X_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/*___________________________ EXPORTED FUNCTIONS ____________________________*/
/*****************************************************************************/
/*            SUBROUTINE:    ow_init()                                       */
/*                                                                           */
/* Purpose:   initializes Setup                                              */
/* Input:     Port: GPIOA-GPIOE, Pin: 0-15                                   */
/* return:	                                                                 */
/*****************************************************************************/
EXPORT void ow_init(GPIO_TypeDef *GPIOx,int Pin);

/*****************************************************************************/
/*            SUBROUTINE:    ow_read_bit()                                   */
/*                                                                           */
/* Purpose:   Read Bit                                                       */
/*            This function will read one single bit from the bus.           */
/* Input:                                                                    */
/* return:	                                                                 */
/*****************************************************************************/
EXPORT uint8_t ow_read_bit(void);

/*****************************************************************************/
/*            SUBROUTINE:    ow_read_byte()                                  */
/*                                                                           */
/* Purpose:   Read Byte                                                      */
/*            This function will read a complete byte from the bus.          */
/* Input:                                                                    */
/* return:	                                                                 */
/*****************************************************************************/
EXPORT uint8_t ow_read_byte(void);

/*****************************************************************************/
/*            SUBROUTINE:    ow_write_bit()                                  */
/*                                                                           */
/* Purpose:   Write Bit                                                      */
/*            This function will write one single bit on the bus.            */
/* Input:                                                                    */
/* return:	                                                                 */
/*****************************************************************************/
EXPORT void ow_write_bit(uint8_t val);

/*****************************************************************************/
/*            SUBROUTINE:    ow_write_byte()                                 */
/*                                                                           */
/* Purpose:   Write Byte                                                     */
/*            This function will write a complete byte on the bus.           */
/* Input:                                                                    */
/* return:	                                                                 */
/*****************************************************************************/
EXPORT void ow_write_byte(uint8_t val);

/******************************************************************************/
/*            SUBROUTINE:    ow_send_mri()                                    */
/*                                                                            */
/* Purpose:   Send Master-Reset-Impuls                                        */
/*            This function will release the master reset impuls and checks   */
/*            if there is at least one 1-wire-component connected             */
/*            to the bus (0) or not (1).                                      */
/* Input:                                                                     */
/* return:	                                                                  */
/******************************************************************************/
EXPORT uint8_t ow_send_mri(void);

#undef EXPORT
#endif /* __ONEWIRE_STM32F10X_H */
