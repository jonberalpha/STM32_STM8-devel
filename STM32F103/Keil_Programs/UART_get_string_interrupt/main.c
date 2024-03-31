#include "USART_STM32F10X.h"
#include <string.h>

// Defines
#define LINEMAX 100 // Maximal allowed/expected line length

// Global Variables
volatile char line_buffer[LINEMAX + 1]; // Holding buffer with space for terminating NUL
volatile int line_valid = 0;

// Prototypes
void NVIC_init(char position, char priority);

//ISR
void USART1_IRQHandler(void)
{ 
  // echo:
  // uart_put_char(uart_get_char()); //Zeichen auslesen
  // return;
  // pending-bit wird automatisch durch Lesen des UART Datenregister (USART1->DR) zurückgesetzt
  
  static char rx_buffer[LINEMAX]; // Local holding buffer to build line
  static int rx_index = 0;
  
  char rx = uart_get_char();
  if((rx == '\r') || (rx == '\n')) // Is this an end-of-line condition, either will suffice?
  {
    if(rx_index != 0) // Line has some content?
    {
      memcpy((void *)line_buffer, rx_buffer, rx_index); // Copy to static line buffer from dynamic receive buffer
      line_buffer[rx_index] = 0; // Add terminating NUL
      line_valid = 1;            // flag new line valid for processing
      rx_index = 0;              // Reset content pointer
    }
  }
  else
  {
    if(rx_index == LINEMAX) // If overflows pull back to start
      rx_index = 0;
    rx_buffer[rx_index++] = rx; // Copy to buffer and increment
  }
}

void NVIC_init(char position, char priority)
{  
  NVIC->IP[position] = (priority<<4);  //Setzen der Interrupt Prioritaet
  NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F)); //Verhindert, dass der Interrupt ausloest sobald er enabled wird 
  NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F)); //Enable Interrupt
} 

int main()
{  
  uart_init(9600);
  NVIC_init(USART1_IRQn, 2);
  USART1->CR1 |= 0x0020; // USART1 RxNE - Interrupt Enable
  
  uart_put_string("Ready...\r\n");
  
  while(1)
  {
    __WFI(); // wait for interrupt
    if(line_valid == 1)
    {
      uart_put_string((char*)line_buffer);
      uart_put_string("\r\n");
      line_valid = 0; // clear pending flag
    }
  }
}
