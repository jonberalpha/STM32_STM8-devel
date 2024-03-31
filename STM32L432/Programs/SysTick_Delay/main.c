#include <stm32l432xx.h>

static __IO uint32_t usTicks;

void init_ports(void);
void delay_init(void);
void delay_us(uint32_t ms);
void delay_ms(uint32_t ms);
void set_clock_80MHz(void);

void SysTick_Handler()
{
  if(usTicks != 0)
  {
    usTicks--;
  }
}

void delay_init(void)
{
  // Update SystemCoreClock value
  SystemCoreClockUpdate();
  // Configure the SysTick timer to overflow every 1 us
  SysTick_Config(SystemCoreClock/1000000);
}

void delay_us(uint32_t us)
{
  // Reload us value
  usTicks = us;
  // Wait until usTick reach zero
  while(usTicks);
}

void delay_ms(uint32_t ms)
{
  // Wait until ms reach zero
  while(ms--)
  {
    // Delay 1ms
    delay_us(1000);
  }
}

void init_ports(void)
{
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
  GPIOB->MODER &= ~0xF0;
  GPIOB->MODER |= 0x40;
  GPIOB->OTYPER &= ~0x8;
  GPIOB->OSPEEDR &= ~0xC0;
  GPIOB->PUPDR &= ~0xC0;
}

void set_clock_80MHz(void)
{
  FLASH->ACR |= 1<<2;

  RCC->CR |= 1<<3;

  RCC->PLLCFGR &= ~(1<<12);
  RCC->PLLCFGR |= ((1<<13) | (1<<11)); // PLLN = 40
  RCC->PLLCFGR |= (1<<0); // PLLSRC = MSI

  RCC->PLLSAI1CFGR |= (1<<25); //PLLSAI1R = 4
  RCC->PLLSAI1CFGR &= ~(1<<12);
  RCC->PLLSAI1CFGR |= ((1<<8) | (1<<9) | (1<<11) | (1<<13)); //PLLSAI1N = 43

  RCC->CR |= (1<<24); // Enable: PLL
  while(((RCC->CR) & (1<<25)) == 0); //Wait till PLL is ready

  RCC->CR |= (1<<26); // Enable: PLLSAI1
  while(((RCC->CR) & (1<<27)) == 0); // Wait till PLLSAI1 is ready

  RCC->PLLCFGR |= (1<<24); // Enable: PLLREN Goes into System Clock 
  RCC->PLLSAI1CFGR |= (1<<24);  //Enable: PLLSAI1REN Goes into ADC

  RCC->CFGR |= ((1<<1) | (1<<0));
  
  SystemCoreClockUpdate(); // update SystemCoreClock-variable
  //SysTick_Config(SystemCoreClock/1000);
}

int main(void)
{
  set_clock_80MHz();
  delay_init();
  init_ports();
  
  while(1)
  {
    GPIOB->ODR |= 0x8;
    delay_ms(500);
    GPIOB->ODR &= ~0x8;
    delay_ms(500);
  }
}
