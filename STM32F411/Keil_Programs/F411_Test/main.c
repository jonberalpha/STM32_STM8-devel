#include <stm32f411xe.h>

#define GPIOB_ODR GPIOB_BASE + 5*sizeof(uint32_t)
#define BITBAND_PERI(a,b)  ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))
#define PB3    *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,3)))  // PB8

static void init_ports(void);

static void init_ports(void)
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  GPIOB->MODER &= ~0xF0;
  GPIOB->MODER |= 0x40;
  GPIOB->OTYPER &= ~0x8;
  GPIOB->OSPEEDR &= ~0xC0;
  GPIOB->PUPDR &= ~0xC0;
}

int main(void)
{
  init_ports();


#define BITBANDING 1

#if BITBANDING
  PB3 = 1;
#else
  GPIOB->ODR |= 0x8; // PB3
#endif
  
  while(1);
}
