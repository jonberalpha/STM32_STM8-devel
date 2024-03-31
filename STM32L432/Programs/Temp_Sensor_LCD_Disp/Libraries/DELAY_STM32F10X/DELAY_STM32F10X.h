#ifndef DELAY_STM32F10X_H_INCLUDED
#define DELAY_STM32F10X_H_INCLUDED

/*________________________________ INCLUDES __________________________________*/
#include <stm32f10x.h>

#ifdef  DELAY_STM32F10X_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/*____________________________ EXPORTED FUNCTIONS ____________________________*/
EXPORT void delay_init(void);
EXPORT void delay_us(uint32_t us);
EXPORT void delay_ms(uint32_t ms);

#undef EXPORT
#endif /* __DELAY_STM32F10X_H */
