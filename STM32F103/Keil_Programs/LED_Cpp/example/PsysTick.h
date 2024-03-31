#ifndef __PDELAY_H
#define __PDELAY_H 
#include "stm32f10x.h"

class PSysTick
{

  public:
    PSysTick();
    ~PSysTick();
  public:
    void Delay(uint32_t nTime);
  void SysTick_Init();
    static uint32_t TimingDelayx;
};
#endif

#include "PsysTick.h"
PSysTick::PSysTick()
{}
PSysTick::~PSysTick()
{}
    
void PSysTick::Delay(uint32_t nTime)
{
  TimingDelayx = nTime;
  while(TimingDelayx != 0);
}

void PSysTick::SysTick_Init()
{
  while(SysTick_Config(SystemCoreClock/1000));
}

uint32_t PSysTick::TimingDelayx=0;


extern "C"
{
  void SysTick_Handler(void)
  {
      if(PSysTick::TimingDelayx!=0x00)
      {
        PSysTick::TimingDelayx--;
      }
      else
        PUsart::SendData('E');
  }
}
