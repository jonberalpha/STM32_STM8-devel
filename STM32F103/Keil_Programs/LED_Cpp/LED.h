#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

#include <stm32f10x.h> // nicht wichtig

#define BITBANDING 1

#define GPIOC_ODR GPIOC_BASE + 3*sizeof(uint32_t)   // Calc peripheral address GPIOB ODR

// Calc Bit Band Adress from peripheral address: a = peripheral address b = Bit number
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))
#define PIN13  *((volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,13)))  // PC13 | name LED or PC13 is not allowed cause of classname LED and object PC13 created in main

class LED
{  
  public:
    // Konstruktor
    LED(int value)
    {
      initLed();
      setLed(value);
    }
    
    void initLed(void)
    {
      RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
      GPIOC->BRR = (1 << 13);
      GPIOC->CRH &= ~0x000F00000; //Konfig-Bits von PC13 auf 0 setzen
      GPIOC->CRH |= 0x000300000; //PC13 auf GPOPP setzen
    }
    
    void setLed(int value)
    {
#if BITBANDING
      PIN13 = value;
#else
      if(value == 1)
      {
        GPIOC->BSRR = (1 << 13);
      }
      else
      {
        GPIOC->BRR = (1 << 13);
      }
#endif
    }
    
    void toggleLed(void)
    {
#if BITBANDING
      PIN13 = ~PIN13;
#else
      GPIOC->ODR ^= (1 << 13);
#endif
    }
};

#endif // LED_H_INCLUDED
