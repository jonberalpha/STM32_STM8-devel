#include <stm8s.h>

/*
#define PB_ODR    *(unsigned char*)0x5005
#define PB_DDR    *(unsigned char*)0x5007
#define PB_CR1    *(unsigned char*)0x5008
*/

// Unsigned int is 16 bit in STM8.
// So, maximum possible value is 65536.
unsigned long int dl; // Delay

int main() {
  GPIOB->ODR = 0x00;  // Turn all pins of port D to low
  GPIOB->DDR |= 1 << 5; // 0x00100000 PB5 is now output
  GPIOB->CR1 |= 1 << 5; // 0x00100000 PB5 is now pushpull

  while(1) {
    GPIOB->ODR ^= 1 << 5; // Toggle PB5 output

    for(dl = 0; dl < 18000; dl++) {}
  }
}

