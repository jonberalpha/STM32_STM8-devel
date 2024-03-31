#include <stm32l432xx.h>

#include "stdio.h"
#include "stdarg.h"

/*____________________________ DEFINES _______________________________________*/
#define ASCII_ESC    27     // \033 or \x1b | CMD Start
#define ASCII_CLRSCR "[2J"  // whole cmd-string: \033[2J; \x1b[2J
#define ASCII_HOME   "[H"
#define ASCII_SVRPOS "[s"
#define ASCII_RESPOS "[u"

static __IO uint32_t usTicks;

void set_clock_80MHz(void);
unsigned int adc_read_poti(void);
void init_ports(void);

void delay_init(void);
void delay_us(uint32_t ms);
void delay_ms(uint32_t ms);

void uart_init(unsigned long baud);
void uart_clear(void);
void uart_put_char(char ch);
void uart_put_string(char *string);

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
  //SysTick_Config(SystemCoreClock/1000);
}

void uart_init(unsigned long baud)
{
  //PA9 out TX/PA10 in RX
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;          //GPIOA CLK enablen
  GPIOA->MODER &= ~0x3C0000;                     //Konfig-Bits von PA9 und PA10 loeschen
  GPIOA->MODER |= 0x280000;                     //PA9 und PA10 als AF
  
  GPIOA->OSPEEDR &= ~0x3C0000;                  //Speed loeschen
  GPIOA->OSPEEDR |= 0x3C0000;                    //Highest speed
  
  GPIOA->AFR[1] |= 0x770;                        //Alternate function: AF7 (111) for USART1
  
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;          //enable UART clock
  USART1->BRR = SystemCoreClock/baud;           //SystemCoreClock(z.B.: 8MHz/72MHz)/Baudrate
  USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE); //TxD und RxD enablen
  USART1->CR2 &= ~USART_CR2_STOP;                //1 Stop bit
  USART1->CR1 |= USART_CR1_UE;                   //USART1 enablen
}

void uart_clear(void)
{
  // Clear Screen ...
  uart_put_char(ASCII_ESC);
  uart_put_string(ASCII_CLRSCR);
  // ... go to home position
  uart_put_char(ASCII_ESC);
  uart_put_string(ASCII_HOME);
}

void uart_put_char(char ch)
{
  while(!(USART1->ISR & USART_ISR_TXE));
  USART1->TDR = ch;
}

void uart_put_string(char *string)
{
  while(*string)
  {
    uart_put_char(*string++);
  }
}

void init_ports(void)
{
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
  GPIOB->BRR = (1 << 3);
  GPIOB->MODER &= ~0xF0;
  GPIOB->MODER |= 0x40;
}

unsigned int adc_read_poti(void)
{
  int pin = 7;
  int channel = 12;
  
  RCC->CCIPR |= RCC_CCIPR_ADCSEL; // set sysclk as adc clock
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable peripheral clock
  GPIOA->MODER |= (0x3 << pin); // set PAX in analog mode
  
  RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;  //enabele peripheral clock for ADC1
  ADC1->CR &= ~ADC_CR_DEEPPWD; // disable Deep sleep mode
  ADC1->CR |= ADC_CR_ADVREGEN; // enable Voltage regulator
  ADC1->CFGR |= ADC_CFGR_JQDIS; // disable injected qeue disable
  ADC1->CFGR &= ~(ADC_CFGR_RES | ADC_CFGR_ALIGN | ADC_CFGR_CONT); // right data alignment, SWSTART Conv., Single Mode
  
  if(channel <= 9)
  {
    ADC1->SMPR1 &= ~(0x7 << channel*3);//(0x5 << channel*3); // (3 cycles)
  }
  else
  {
    ADC1->SMPR2 &= ~(0x7 << (channel-10)*3);//(0x5 << channel*3);////(0x5 << channel*3); //set sample time ADC1 channel x (92,5 cycles)
  }
  
  ADC1->SQR1 &= ~0xF; // single conversion in L register, set channel as first in regular conv. sequence
  ADC1->SQR1 |= (channel << 0x6); //only one conversion item in regular sequence
  ADC1->SQR2 = 0x00000000;
  ADC1->SQR3 = 0x00000000;
  
  //ADC1->ISR |= 0x1; // Clear the ADRDY bit in the ADC_ISR register by writing '1'
  
  
  ADC1->CR |= ADC_CR_ADEN; // enable ADC (ADON)
  while((ADC1->ISR & ADC_ISR_ADRDY) == 0); // Wait until ADRDY=1
  
  ADC1->CR |= ADC_CR_ADSTART; //start conversion by software for regular sequence
  while((ADC1->ISR & ADC_ISR_EOC) == 0);//Wait till conversion is finished (EOC=1)
  
  return(ADC1->DR); //deliver converted value of channel
}

int main(void)
{
  unsigned int adcvalue = 0;
  char buf[20] = "";
  
  set_clock_80MHz();
  init_ports();
  delay_init();
  uart_init(9600);
  
  while(1)
  {
    GPIOB->ODR ^= 0x8;
    adcvalue = adc_read_poti(); // channel 12, ADC1 for PA7
    sprintf(buf, "%d\r\n", adcvalue);
    uart_put_string(buf);
    delay_ms(500);
  }
}
