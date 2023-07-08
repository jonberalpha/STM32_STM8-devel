#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

/*________________________________ INCLUDES _________________________________*/
#include <stm32f10x.h>

/*________________________________ DEFINES __________________________________*/
#define LINEMAX 100

/*______________________________ BITBANDING _________________________________*/
//Calc peripheral address GPIOA ODR
#define GPIOA_ODR GPIOA_BASE + 3*sizeof(uint32_t)
//Calc Bit Band Adress from peripheral address: a = peripheral address b = Bit number:
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))
#define LEDr  *((volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,2))) // PA2
#define LEDg  *((volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,1))) // PA1
#define LEDb  *((volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,0))) // PA0

#endif /* __MAIN_H */
