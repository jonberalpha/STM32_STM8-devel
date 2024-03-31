#include <stm32f10x.h>
#include <stdarg.h>
#include <stdio.h>

//Prototypes
void set_clock_72MHz(void);
void uart_init(unsigned long baud);
void uart_put_char(char c);
void uart_put_string(char *string);
void uart_printf(const char *format, ...);
char uart_get_char(void);
void TIM1_config(void);
void NVIC_init(char position, char priority);

//Globale Variablen
volatile int signal = 0;

//ISRs
void TIM1_UP_IRQHandler(void)
{
  TIM1->SR &= ~0x1;
  
  uart_put_string("Hallo \r"); //Echo
}

//Unterprogramme
void set_clock_72MHz(void)
{
  FLASH->ACR = 0x12; //Set Flash latency (2 waitstates)
  RCC->CR |= RCC_CR_HSEON; //HSE on
  
  while((RCC->CR & RCC_CR_HSERDY) == 0); // Wait for HSERDY=1 (HSE is ready)
  
  RCC->CFGR |= RCC_CFGR_PLLMULL9; // 9 mal 8 = 72 MHz (SYSCLK)
  RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6; //ADCCLK = SYSCLK/6 (APB2 PRESCALER=1)
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // PCLK1(APB1)=SYSCLK/2 (HCLK=SYSCLK)
  RCC->CFGR |= RCC_CFGR_PLLSRC; //PLL = SYSCLK,HCLK=SYSCLK,da AHB PRESCALER=1)
  RCC->CR |= RCC_CR_PLLON; //PLL on
  
  while((RCC->CR & RCC_CR_PLLRDY) == 0); // Wait for PLLRDY=1 (PLL is ready)
  
  //RCC->CFGR |= RCC_CFGR_SW_HSE; //HSE = Systemclock
  RCC->CFGR |= RCC_CFGR_SW_PLL; //PLL = Systemclock
  
  while ((RCC->CFGR & RCC_CFGR_SWS_PLL) == 0);
  
  /* Wait till SYSCLK is stabilized (depending on selected clock) */
  while ((RCC->CFGR & RCC_CFGR_SWS) != ((RCC->CFGR<<2) & RCC_CFGR_SWS));
  
  // end of stm32_ClockSetup
  RCC->BDCR |=RCC_BDCR_LSEON; //32kHz für RTC siehe AN2821 Reference Manual448ff/1072
  
  SystemCoreClockUpdate();
}

void uart_init(unsigned long baud)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;            //GPIOA CLK enablen
  GPIOA->CRH &= ~0x0FF0;                         //Konfig-Bits von PA9 und PA10 loeschen
  GPIOA->CRH |= 0x04B0;                         //PA9 als AFOPP und PA10 als IPF
  
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  USART1->BRR = SystemCoreClock/baud;           //SystemCoreClock(z.B.: 8MHz/72MHz)/Baudrate
  USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE); //TxD und RxD enablen
  USART1->CR1 |= USART_CR1_UE;                   //USART1 enablen
}

void uart_put_char(char c)
{
  while(!(USART1->SR & USART_SR_TXE));
  USART1->DR = c;
}

void uart_put_string(char *string)
{
  while(*string)
  {
    uart_put_char(*string++);
  }
}

void uart_printf(const char *format, ...)      //UART-Printf - ACHTUNG: max. Länge!
{
  static char buffer[1024];                    //Buffer
  va_list  argptr;                            //Argument-Liste
  va_start( argptr, format );
  vsprintf( buffer, format, argptr );          //Mit sprintf-Funktion in Buffer übertragen
  va_end  ( argptr );
  buffer[1024-1]=0;                            //Zur Sicherheit
  uart_put_string(buffer);
}

char uart_get_char(void)
{
  char c;
  
  while(!(USART1->SR & USART_SR_RXNE));
  c = USART1->DR;
  return c;
}

void TIM1_config(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
  TIM1->SMCR &= ~0x7;
  TIM1->CR1 &= ~0x70;
  TIM1->PSC = 8000;
  TIM1->ARR = 1000;
  TIM1->DIER |= 0x1;
  NVIC_init(TIM1_UP_IRQn, 1);
  TIM1->CR1 |= 0x1;
}

void NVIC_init(char position, char priority) 
{  
  NVIC->IP[position]=(priority<<4);  //Setzen der Interrupt Prioritaet
  NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F)); //Verhindert, dass der Interrupt ausloest sobald er enabled wird 
  NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F)); //Enable Interrupt
} 

int main(void)
{
  set_clock_72MHz();
  uart_init(9600);
  uart_printf("SystemCLK: %d", SystemCoreClock);
  uart_put_string("\r\n");
  TIM1_config();
  
  while(1);
}
