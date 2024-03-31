#include <armv10_std.h>
#include "EA_DOGM204_A.h"

int main (void)
{
  //Init Cortex-Display
  lcd_init();
  lcd_clear();
  
  //Init EA_DOGM204_A-Display
  LCD_Init();
  LCD_SetPosition(BOTTOMVIEW);
  LCD_Clear();
  
  //lcd_put_char('-');
  //lcd_set_cursor(0,2);    //1. zeile Position 3
  //sprintf(&buffer[0], "RM-Messger\xe1t");
  //lcd_put_string(&buffer[0]);
  lcd_set_cursor(0,0);
  lcd_put_string("Schau aufs");
  lcd_set_cursor(1,0);
  lcd_put_string("untere Display");
  
  LCD_WriteString("Schau aufs obere    Display");
}
