#ifndef UART_STM32F10X_H_INCLUDED
#define UART_STM32F10X_H_INCLUDED

/*________________________________ INCLUDES __________________________________*/
#include <stm32f10x.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef  UART_STM32F10X_MOD
#define EXPORT
#else
#define EXPORT extern
#endif 

/*____________________________ EXPORTED FUNCTIONS ____________________________*/
EXPORT void uart_init(unsigned long baud);
EXPORT void uart_clear(void);
EXPORT void uart_put_char(char ch);
EXPORT void uart_put_string(char *string);
EXPORT char uart_get_char(void);


#undef EXPORT
#endif /* __UART_STM32F10X_H */
