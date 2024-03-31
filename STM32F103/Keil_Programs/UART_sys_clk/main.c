#include "USART_STM32F10X.h"
#include "SYS_CLK.h"
#include "WAIT_STM32F10X.h"
#include "stdio.h"

int main()
{
  set_clock_72MHz();
  char buffer[20];
  
  wait_init();
  uart_init(9600);
  
  int mySystemCoreClock = SystemCoreClock;
  sprintf(buffer,"%d",mySystemCoreClock);
  
  while(1)
  {
    uart_put_string(buffer);
    uart_put_string("\r");
    wait_ms(1000);
  }
}
