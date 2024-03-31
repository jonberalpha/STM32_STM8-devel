#include <stm32f10x.h>
extern "C" {
  #include "WAIT_STM32F10X.h"
}

#include "LED.h"

int main()
{
  wait_init();
  LED PC13 = LED(0); // mit 0 initialisieren = LED leuchtet nicht

  while(1)
  {
    PC13.setLed(1);
    wait_ms(500);
    PC13.setLed(0);
    wait_ms(500);
    PC13.toggleLed();
    wait_ms(500);
  }
}
