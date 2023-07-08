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
EXPORT void i2c_init(I2C_TypeDef* registerStruct, bool remap, bool fastMode, 
            uint32_t apb1_clock);
EXPORT int i2c_communicate(I2C_TypeDef* registerStruct, uint8_t slave_addr, 
           uint8_t* send_buffer, int send_size, uint8_t* receive_buffer, int receive_size);

#undef EXPORT
#endif /* __I2C_STM32F10X_H */
