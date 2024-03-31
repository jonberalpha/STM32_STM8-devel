#define UART_STM32F10X_MOD
#include "UART_STM32F10X.h"

/*____________________________ DEFINES _______________________________________*/
#define ASCII_ESC    27     // \033 or \x1b | CMD Start
#define ASCII_CLRSCR "[2J"  // whole cmd-string: \033[2J; \x1b[2J
#define ASCII_HOME   "[H"
#define ASCII_SVRPOS "[s"
#define ASCII_RESPOS "[u"

/*_______________________ PRIVATE PROTOTYPES _________________________________*/
static void NVIC_init(char position, char priority);

/*___________________________ SUBROUTINES ____________________________________*/
void uart_init(unsigned long baud)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;  					//GPIOA CLK enablen
	GPIOA->CRH &= ~0x0FF0; 												//Konfig-Bits von PA9 und PA10 loeschen
	GPIOA->CRH |= 0x04B0; 												//PA9 als AFOPP und PA10 als IPF
	
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->BRR = SystemCoreClock/baud; 					//SystemCoreClock(z.B.: 8MHz/72MHz)/Baudrate
	USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE); //TxD und RxD enablen
	USART1->CR1 |= USART_CR1_RXNEIE;              //RX interrupt enable
	
	NVIC_init(USART1_IRQn, 2);                    //Interrupt enablen
	
	USART1->CR1 |= USART_CR1_UE; 									//USART1 enablen
}

static void NVIC_init(char position, char priority)
{	
	NVIC->IP[position] = (priority<<4);	//Setzen der Interrupt Prioritaet
	NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F)); //Verhindert, dass der Interrupt ausloest sobald er enabled wird 
	NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F)); //Enable Interrupt
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
	while(!(USART1->SR & USART_SR_TXE));
	USART1->DR = ch;
}

void uart_put_string(char *string)
{
	while(*string)
	{
		uart_put_char(*string++);
	}
}

char uart_get_char(void)
{
	char ch;
	
	while(!(USART1->SR & USART_SR_RXNE));
	ch = USART1->DR;
	return ch;
}
