#include "USART_STM32F10X.h"
#include "WAIT_STM32F10X.h"

int main()
{
  int i=0;
  char buffer[100];
  
  wait_init();
  uart_init(9600);
  
  while(1)
  {
    uart_put_string("Hallo\r\n");
    uart_get_string(buffer);
    uart_put_string("\r\n\r\n");
    uart_put_string("Folgender String wurde uebertragen: ");
    uart_put_string(buffer);
    uart_put_string("\r\n\r\n");
    if(i==2)
    {
      uart_clear();
      i=0;
    }
    else
    {
      i++;
    }
  }
}
