#include <stdint.h>
#include <stm8s_all.h>

ISR(tim1_isr, TIM1_OVR_UIF_vector)
{
  TIM1_SR1 &= ~0x01;
  PB_ODR ^= (1 << 5); // Toggle PB5 output
}

void TIM1_init(void)
{
  TIM1_CR1 &= ~(0x1 << 4); // Up-Counter
  TIM1_CR1 |= (0x1 << 7); // Enable automatic reloading
  TIM1_CR1 |= (0x1 << 2); // USR
  
  // At 16MHz, we set the prescaler to 16000 (0x3E80).
  // This gives us 16MHz / 16k = 1kHz, i.e. 1000 ticks per second. -> ARR = 1000
  TIM1_PSCRH = 0x3E;
  TIM1_PSCRL = 0x80;
  
  TIM1_ARRH = 0x03;
  TIM1_ARRL = 0xE8;
  
  TIM1_IER |= 0x1;
  //TIM1_EGR |= 0x1;
  
  TIM1_CR1 |= 0x1; // enable counter
}

void GPIO_init(void)
{
  PB_ODR = 0x20;  // Turn all pins of port B to high wich is low
  PB_DDR |= 1 << 5; // 0x00100000 PB5 is now output
  PB_CR1 |= 1 << 5; // 0x00100000 PB5 is now pushpull
}

int main()
{
  CLK_CKDIVR = 0x00;
  
  GPIO_init();
  TIM1_init();
  
  enableInterrupts();

  while(1);
}

