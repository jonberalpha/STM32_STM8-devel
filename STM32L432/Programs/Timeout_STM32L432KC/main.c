/****** INCLUDES *******/
#include <stm32l432xx.h>

/****** DEFINES *******/
#define US 1
#define MS 2

/****** GLOBAL VARIABLES *******/
int timticks = 0;
void (*funcp)();

/****** PROTOTYPES *****/
void set_clock_80MHz(void);
void TIM1_init(void);
void TIM2_init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

void _tim_timeout_blocking(uint32_t time, int unit);
void _tim_timeout_nonblocking_with_callback(uint32_t time, uint16_t timeunit, void (*func)());

void init_ports(void);
void blinky(GPIO_TypeDef *port, uint32_t pin, uint32_t dutycycle, int timeunit); // 0-100, e.g. 10ms/90ms
uint32_t buttoncheck(GPIO_TypeDef *port, uint32_t pin);
void setlet(void);

/****** ISRs *********/
void TIM1_UP_TIM16_IRQHandler(void) // for blocking-timeout
{
  TIM1->SR &= ~TIM_SR_UIF; // clear pending bit to avoid recalling TIM_ISR
  
  timticks++;
}

void TIM2_IRQHandler(void) // for nonblocking-timeout
{
  static int sw = 0;
  
  TIM2->SR &= ~TIM_SR_UIF; // clear pending bit to avoid recalling TIM_ISR
  
  if(sw >= 1) // switch, da beim enablen gleich die ISR ausgefuehrt wird
  {
    // execute function wich shall be delayed
    funcp(); // executes whatever function assigned early on
    sw = 2;
    TIM2->CR1 &= ~TIM_CR1_CEN; // disable Timer 2
  }
  else
  {
    sw++;
  }
  
}

void set_clock_80MHz(void)
{
  FLASH->ACR |= 1<<2;

  RCC->CR |= 1<<3;

  RCC->PLLCFGR &= ~(1<<12); //
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
  //SysTick_Config(SystemCoreClock/1000); // -> 1ms
}

void setled(void)
{
  GPIOB->BSRR = (1<<3); // run-indication LED
}

void TIM1_init(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // enable TIMER 1 - Clock
  SystemCoreClockUpdate(); // refresh SystemCoreClock-variable
  
  TIM1->PSC = 0;
  TIM1->ARR = SystemCoreClock/1000000 - 1; // 80MHz/1MHz=80 -> alle 1us Interrupt
  TIM1->CR1 |= TIM_CR1_URS;   // set URS-flag: only overflow/underflow generates update interrupt
  TIM1->DIER |= TIM_DIER_UIE; // enable Update interrupt
  //TIM1->EGR |= 0x1;   // enable Timer 1
  
  NVIC_SetPriority(TIM1_UP_TIM16_IRQn,8); // less priority than timer 2
  NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
}

void TIM2_init(void)
{
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // enable TIMER 2 - Clock
  SystemCoreClockUpdate(); // refresh SystemCoreClock-variable
  
  TIM2->PSC = SystemCoreClock/1000000 - 1; // prescale 80MHz to 80
  TIM2->ARR = 0xFFFFFFFF - 1; // ARR Wert maximal 32bit Register ... wird vom user sowieso veraendert
  TIM2->CR1 |= TIM_CR1_URS;   // set URS-flag: only overflow/underflow generates update interrupt
  TIM2->DIER |= TIM_DIER_UIE; // enable Update interrupt
  //TIM2->EGR |= 0x1;   // enable Timer 2
  
  NVIC_SetPriority(TIM2_IRQn,7);
  NVIC_EnableIRQ(TIM2_IRQn);
}

void delay_us(uint32_t us)
{
  TIM1->CR1 |= TIM_CR1_CEN; // enable Timer 1
  timticks = 0;
  while(timticks < us) // blocking loop
  {
    __WFI(); // sleep until interrupt occur; in this case wait until Timerinterrupt occurs
  }
  TIM1->CR1 &= ~TIM_CR1_CEN; // after loop disable Timer 1
}

void delay_ms(uint32_t ms)
{
  TIM1->CR1 |= TIM_CR1_CEN; // enable Timer 1
  timticks = 0;
  while(timticks < (ms*1000)) // blocking loop
  {
    __WFI(); // sleep until interrupt occur; in this case wait until Timerinterrupt occurs
  }
  TIM1->CR1 &= ~TIM_CR1_CEN; // after loop disable Timer 1
}

void _tim_timeout_blocking(uint32_t time, int unit)
{
  timticks = 0;
  //__DSB(); // avoke Data Memory Barrier
  if(unit == US)
  {
    delay_us(time);
  }
  if(unit == MS)
  {
    delay_ms(time);
  }
}

void _tim_timeout_nonblocking_with_callback(uint32_t time, uint16_t unit, void (*func)())
{
  funcp = func;

  if(unit == US)
  {
    TIM2->PSC = SystemCoreClock/1000000 - 1; // clock auf 1us reduziert: 80 - 1
    TIM2->ARR = time; // every 10 counts 1ms (fsys = 80MHz) -> ISR; given MS * 10 because every 10 count equals 1ms
  }
  if(unit == MS)
  {
    // change PSC and ARR to fit MS
    TIM2->PSC = SystemCoreClock/10000 - 1; // clock auf 10kHz(=100us) reduziert: 80MHZ/10000 -> PSC =8000-1
    TIM2->ARR = 10*time; // alle 10*100us = 1ms (fsys = 80MHz)
  }
  
  TIM2->CR1 |= TIM_CR1_CEN; // enable Timer 2
}

void init_ports(void)
{
  // init LED3 on PB3
  RCC->AHB2ENR   |= (1<<1); // enable GPIOB-Clock
  
  GPIOB->MODER   &= ~0xC0;  // delete Mode Config-bits for PB3
  GPIOB->MODER   |= 0x40;   // set PB3 as Mode 01: GPOM (=General purpose output mode)
  
  GPIOB->OTYPER  &= ~0x8;   // write 0 to use Output push-pull
  GPIOB->OSPEEDR &= ~0xC0;  // write 00 for low speed
  GPIOB->PUPDR   &= ~0xC0;  // no pull-up or pull-down
  
  GPIOB->BRR = (1<<3);      // set default state of LED as off
  
  
  // init button (connected to A2 on the click shield) on PA3
  RCC->AHB2ENR   |= (1<<0); // enable GPIOA-Clock
  
  GPIOA->MODER   &= ~0xC0;  // set PA3 as Mode 00: IM (=Input mode)
  GPIOA->PUPDR   &= ~0xC0;  // no pull-up or pull-down for PA3, external pull-up exists
  GPIOA->PUPDR   |= 0x40;   // set pull-up
  
  
  // init RGB-LED (low active): R-A5(PA6) G-D9(PA8) B-A3(PA4)
  // the following line is unnecessary because RCC is already set
  RCC->AHB2ENR   |= (1<<0); // enable GPIOA-Clock
  
  GPIOA->MODER   &= ~0x3000;    // delete Mode Config-bits for PB3
  GPIOA->MODER   |= 0x1000;     // set PA6 as Mode 01: GPOM (=General purpose output mode)
  
  GPIOA->MODER   &= ~0x30000;   // delete Mode Config-bits for PB3
  GPIOA->MODER   |= 0x10000;    // set PA8 as Mode 01: GPOM (=General purpose output mode)
  
  GPIOA->MODER   &= ~0x300;     // delete Mode Config-bits for PB3
  GPIOA->MODER   |= 0x100;      // set PA4 as Mode 01: GPOM (=General purpose output mode)
  
  
  GPIOA->OTYPER  &= ~((1<<6) | (1<<8) | (1<<4));   // write 0 to use Output push-pull
  GPIOA->OSPEEDR &= ~0x33300;                      // write 00 for low speed
  GPIOA->PUPDR   &= ~0x33300;                      // no pull-up or pull-down
  
  GPIOA->ODR |= ((1<<6) | (1<<8) | (1<<4));        // set default state of LED as off
}

void blinky(GPIO_TypeDef *port, uint32_t pin, uint32_t dutycycle, int timeunit)
{
  // preprocessing
  if(dutycycle > 100)
  {
    dutycycle = 100;
  }

  // LED-PWM with blocking delay function
  port->BSRR = (1<<pin);
  if(timeunit == US)
  {
    _tim_timeout_blocking(dutycycle*10, US);      // convert dutycycle in % to actual delaytime for High phase
  }
  if(timeunit == MS)
  {
    _tim_timeout_blocking(dutycycle, MS);
  }
  port->BRR = (1<<pin);
  if(timeunit == US)
  {
    _tim_timeout_blocking(1000-dutycycle*10, US); // convert dutycycle in % to actual delaytime for Low phase
  }
  if(timeunit == MS)
  {
    _tim_timeout_blocking(100-dutycycle, MS);
  }
}

uint32_t buttoncheck(GPIO_TypeDef *port, uint32_t pin)
{
  uint32_t tmp = 0;
  static uint16_t endstate = 0, cnt0 = 0, cnt1 = 0;
  uint16_t delta = 0;

  tmp = (port->IDR & (1<<pin)); // mask IDR with given pin to read state
  tmp = tmp >> pin;             // to get only 1 or 0 shift it to the right according to pin

  // before debounce invert tmp because Debounce is active high but button is active low
  tmp ^= 0x1;

  /* DEBOUNCE */
  // count1, count2, state hold the debounce-state
  // if all 3 states remain stable for 3 samples than the state = 1 (debounced)
  // delta tells the difference from the input to the current debounced state
  delta = tmp ^ endstate;
  cnt1 = (cnt1 ^ cnt0) & delta;
  cnt0 = ~cnt0 & delta;
  endstate = endstate ^ (cnt0 & cnt1);
  /************/

  return endstate;
}

int main(void)
{
  int inc = 1;
  int stop = 0;
  uint32_t btn = 0;
  
  set_clock_80MHz();
  init_ports();
  TIM1_init();
  TIM2_init();
  
  _tim_timeout_nonblocking_with_callback(5000, MS, &setled); // nach 5 Sekunden onboard LED einschalten
  
  while(1)
  {
    btn = buttoncheck(GPIOA, 3);
    if((btn == 1) && (stop == 0)) // Button low active
    {
      if(inc == 10)
      {
        inc = 1;
      }
      else
      {
        inc++;
      }
      stop = 1;
    }
    else
    {
      if(btn == 0)
      {
        stop = 0;
      }
      blinky(GPIOA, 6, 10*inc, US); // multiple of 10 ms (r:PA6/g:PA8/b:PA5/LED3:PB3)
    }
  }
}
