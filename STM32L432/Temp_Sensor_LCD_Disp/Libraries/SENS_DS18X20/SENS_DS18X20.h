#ifndef SENS_DS18X20_H_INCLUDED
#define SENS_DS18X20_H_INCLUDED

/*_______________________________ INCLUDES ___________________________________*/
#include <stm32f10x.h>
#include "DELAY_STM32F10X.h"

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
// Calc peripheral address GPIOB ODR
#define GPIOB_ODR GPIOB_BASE + 3*sizeof(uint32_t)
// Calc peripheral address GPIOB IDR
#define GPIOB_IDR GPIOB_BASE + 2*sizeof(uint32_t) 
// Calc Bit Band Adress from peripheral address: a = peripheral address b = Bit number:
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))

#define DQ_o *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,0))) //DQ out/PB0
#define DQ_i *((volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,0))) //DQ in/PB0

/*____________________________ EXPORTED FUNCTIONS ____________________________*/
EXPORT void DS18X20_init(void);
EXPORT float get_temperature(void);

#undef EXPORT
#endif /* __SENS_DS18X20_H */
