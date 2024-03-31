#define __GPIO_LIB_H_MOD
#include "stm32f10x.h"
#include "gpio_lib.h"

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void init_port(GPIO_TypeDef* GPIO,char from,char to,char mode,char frequency)
{  char value = (mode & 0x0C);                //value: 4bit-Wert, CNF und MODE Daten eintragen
  char i;                             //für Zählschleife und Reihenfolge ändern

  if((mode & 0x10) != 0)                  //Output --> Frequenz setzen
  {  value |= ((frequency!=0)?(frequency&0x03):0x03);  //Frequenz eintragen
  }
  if(to<from)                        //falsche Reichtung -> drehung
  {  i=to;                        //Reihenfolge ändern
    to=from;
    from=1;
  }
  switch((int)GPIO)                     //enable clock für GPIO (APB2 Peripheral clock enable register)
  {  case (int)GPIOA: RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;  break;
    case (int)GPIOB: RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;  break;
    case (int)GPIOC: RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;  break;
    case (int)GPIOD: RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;  break;
    case (int)GPIOE: RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
  }
  for(i=from;i<=to;i++)                  //Für alle gewählten Portpins
  {  if(i<8)                        //Pins von 0-7
    {  GPIO->CRL |=  ( value     << (   i   << 2));  //entsprechende Bits setzen
      GPIO->CRL &=~ ((value^0x0F) << (   i   << 2));  //entsprechende Bits löschen (Value = 4bit-Wert)
    }else                          //Pins von 8-15
    {  GPIO->CRH |=  (  value     << ( (i-8) << 2));  //entsprechende Bits setzen
      GPIO->CRH &=~ ((value^0x0F) << ( (i-8) << 2));  //entsprechende Bits löschen (Value = 4bit-Wert)
    }
    if((mode & 0x20) !=0 )                   //ODR-Wert löschen
    {  GPIO->ODR &=~ ( 0x01 << i);            //bit löschen
    }
    if((mode & 0x40) !=0 )                   //ODR-Wert setzen
    {  GPIO->ODR |=  ( 0x01 << i);            //bit setzen
    }
  }

  if(GPIO == GPIOB && from<=4 && to>=4)          //PA4 betroffen
  {  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;          //Peripherie versorgen
    AFIO->MAPR   |= 0x01000000;              //JTAG konfigurieren dass dieser Pin nicht benutzt wird  (SWJ _CFG=001)
  }
}


/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState)
{
  uint32_t tmp = 0x00, tmp1 = 0x00, tmpreg = 0x00, tmpmask = 0x00;
 
  
  tmpreg = AFIO->MAPR;

  tmpmask = (GPIO_Remap & DBGAFR_POSITION_MASK) >> 0x10;
  tmp = GPIO_Remap & LSB_MASK;

  if ((GPIO_Remap & (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK)) == (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK))
  {
    tmpreg &= DBGAFR_SWJCFG_MASK;
    AFIO->MAPR &= DBGAFR_SWJCFG_MASK;
  }
  else if ((GPIO_Remap & DBGAFR_NUMBITS_MASK) == DBGAFR_NUMBITS_MASK)
  {
    tmp1 = ((uint32_t)0x03) << tmpmask;
    tmpreg &= ~tmp1;
    tmpreg |= ~DBGAFR_SWJCFG_MASK;
  }
  else
  {
    tmpreg &= ~(tmp << ((GPIO_Remap >> 0x15)*0x10));
    tmpreg |= ~DBGAFR_SWJCFG_MASK;
  }

  if (NewState != DISABLE)
  {
    tmpreg |= (tmp << ((GPIO_Remap >> 0x15)*0x10));
  }

  AFIO->MAPR = tmpreg;
}    
