#include <stdio.h>
#include "USART_STM32F10X.h"
#include "WAIT_STM32F10X.h"
#include "DIL_POTI.h"
#include "LCD_DEM16217.h"
#include "SENS_DS18X20.h"

int main(void)
{
  int wert=0;
  char buffer[20];
  // Custom char data (battery symbol)
  uint8_t custom_char[] = { 0x0E, 0x1B, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x1F };
  float temp=0;
  
  uart_init(9600);
  wait_init();
  // LCD initialization
  lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
  // Create custom char
  lcd16x2_create_custom_char(0, custom_char);
  
  uart_put_string("Hallo \r");
  wait_ms(500);
  uart_clear();
  
  DS18X20_init();
  
  while(1)
  {
    wert = poti_convert();
    sprintf(buffer,"%d \r",wert);
    uart_put_string(buffer);
    
    lcd16x2_home(); //goto 0,0
    
    lcd16x2_put_custom_char(0, 0, 0);
    lcd16x2_puts(" Battery Icon");
    
    temp = get_temperature();
    sprintf(buffer,"Temp: %5.1f oC",temp);
    lcd16x2_gotoxy(0,1);
    lcd16x2_puts(buffer);
  }
}
