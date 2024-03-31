.h file--
#ifndef __PLED__
#define __PLED__

#include "stm32f10x.h"

class PLed
{
    public:
        PLed();
        ~PLed();
    public:
        void LED_GPIO_Config(void);
        void TurnOn(uint16_t port,bool status);
      
    private:
};
#endif


#include "PLed.h"
PLed::PLed()
{
  LED_GPIO_Config();
}

PLed::~PLed()
{
  
}

void PLed::LED_GPIO_Config()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_5;    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_SetBits(GPIOE,GPIO_Pin_5);   
  GPIO_SetBits(GPIOE,GPIO_Pin_6); 
}

void PLed::TurnOn(uint16_t port,bool status)
{
  if(status)
        GPIO_SetBits(GPIOE,port);
    else
        GPIO_ResetBits(GPIOE,port);
}
