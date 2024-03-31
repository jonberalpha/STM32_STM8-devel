#include "stm32f10x.h"
#include "misc.h"
#include "stdio.h"
#include "PSysTick.h"
#include "PLed.h"

int main(void)
{        
  PSysTick pst;
  pst.SysTick_Init();

  PLed pled;
  
  while (1)
  {
    pled.TurnOn(GPIO_Pin_5,true);
    pled.TurnOn(GPIO_Pin_6,false);

    pst.Delay(500);
    

    pled.TurnOn(GPIO_Pin_5,false);
    pled.TurnOn(GPIO_Pin_6,true);
    
    pst.Delay(500);
  }
}
