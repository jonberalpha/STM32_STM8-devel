#define __RCC_LIB_MOD
#include "rcc_lib.h"

/*****************************************************************************/
/*                                 Konstanten                                */
/*****************************************************************************/  
__I uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
__I uint8_t ADCPrescTable[4] = {2, 4, 6, 8};


/****************************************************************/
/*      RCC system reset(for debug purpose)          */
/****************************************************************/
void RCC_DeInit(void)//PRI im Original: Setzt alle Takte auf deren Ursprungsszustand zurück.                     
{ 
    RCC->CR |= (uint32_t)0x00000001;            //Set HSION bit
    RCC->CFGR &= (uint32_t)0xF8FF0000;            //Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits
    RCC->CR &= (uint32_t)0xFEF6FFFF;            //Reset HSEON, CSSON and PLLON bits
    RCC->CR &= (uint32_t)0xFFFBFFFF;            //Reset HSEBYP bit
    RCC->CFGR &= (uint32_t)0xFF80FFFF;            //Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits
    RCC->CIR = 0x009F0000;                  //Disable all interrupts and clear pending bits
}

/****************************************************************/
/*      Enable HSE                       */
/****************************************************************/
void RCC_HSEConfig(void)//PRI im Original: Aktiviert den Externen Highspeed Oszillator (Quarz). 
{
   /* Reset HSEON and HSEBYP bits before configuring the HSE ------------------*/
    RCC->CR &= CR_HSEON_Reset;                  //Reset HSEON bit
    RCC->CR &= CR_HSEBYP_Reset;                //Reset HSEBYP bit
    RCC->CR |= CR_HSEON_Set;                //Configure HSE (RCC_HSE_OFF is already covered by the code section above)
}
  
/****************************************************************/
/*      Wait till HSE is ready                */
/****************************************************************/
int RCC_WaitForHSEStartUp(void)
{
  __IO uint32_t StartUpCounter = 0;
    int status = 0;
    int HSEStatus = 0;
  
    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
      HSEStatus = RCC_GetFlagStatus(RCC_FLAG_HSERDY);
      StartUpCounter++;  
    } while((StartUpCounter != HSEStartUp_TimeOut) && (HSEStatus == 0));
  
    if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != 0)
    {
      status = 1;
    }
    else
    {
      status = 0;
   }  
    return (status);
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
int RCC_GetFlagStatus(uint8_t RCC_FLAG)
{ 
    uint32_t tmp = 0;
    uint32_t statusreg = 0;
    int bitstatus = 0;
        
    /* The flag to check is in CR register */
    statusreg = RCC->CR;

    /* Get the flag position */
    tmp = RCC_FLAG & FLAG_Mask;
    if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET)
    {
      bitstatus = 1;
    }
    else
    {
      bitstatus = 0;
    }

    /* Return the flag status */
    return bitstatus;
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void FLASH_PrefetchBufferCmd(void)
{
  //Enable the Prefetch Buffer   
    FLASH->ACR &= ACR_PRFTBE_Mask;
    FLASH->ACR |= FLASH_PrefetchBuffer_Enable;
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void FLASH_SetLatency(void)
{  
    uint32_t tmpreg = 0;
  
    tmpreg = FLASH->ACR;          //Read the ACR register
    tmpreg &= ACR_LATENCY_Mask;    //Sets the Latency value
    tmpreg |= FLASH_Latency_2;
    FLASH->ACR = tmpreg;      //Write the ACR register

}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_HCLKConfig(void)
{ 
    uint32_t tmpreg = 0;

    tmpreg = RCC->CFGR;
    tmpreg &= CFGR_HPRE_Reset_Mask;  //Clear HPRE[3:0] bits
    tmpreg |= RCC_SYSCLK_Div1;    // Set HPRE[3:0] bits according to RCC_SYSCLK value
    RCC->CFGR = tmpreg;        //Store the new value
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_PCLK2Config(void)
{
    uint32_t tmpreg = 0;

    tmpreg = RCC->CFGR;
    tmpreg &= CFGR_PPRE2_Reset_Mask;  //Clear PPRE2[2:0] bits  
    tmpreg |= RCC_HCLK_Div1 << 3;    //Set PPRE2[2:0] bits according to RCC_HCLK value
    RCC->CFGR = tmpreg;          //Store the new value
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_PCLK1Config(void)
{
    uint32_t tmpreg = 0;
    tmpreg = RCC->CFGR;
    tmpreg &= CFGR_PPRE1_Reset_Mask;  //Clear PPRE1[2:0] bits
    tmpreg |= RCC_HCLK_Div2;      //Set PPRE1[2:0] bits according to RCC_HCLK value
    RCC->CFGR = tmpreg;          //Store the new value
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_PREDIV2Config(uint32_t RCC_PREDIV2_Div)
{
    uint32_t tmpreg = 0;

    tmpreg = RCC->CFGR2;
    tmpreg &= ~CFGR2_PREDIV2;      //Clear PREDIV2[3:0] bits
    tmpreg |= RCC_PREDIV2_Div;      //Set the PREDIV2 division factor
    RCC->CFGR2 = tmpreg;        //Store the new value
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_PLL2Config(uint32_t RCC_PLL2Mul)
{
    uint32_t tmpreg = 0;

    tmpreg = RCC->CFGR2;
    tmpreg &= ~CFGR2_PLL2MUL;      //Clear PLL2Mul[3:0] bits
    tmpreg |= RCC_PLL2Mul;        //Set the PLL2 configuration bits
    RCC->CFGR2 = tmpreg;        //Store the new value
} 

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_PLL2Cmd(FunctionalState NewState)
{
  *(__IO uint32_t *) CR_PLL2ON_BB = (uint32_t)NewState;
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_PLL3Config(uint32_t RCC_PLL3Mul)
{
    uint32_t tmpreg = 0;

    tmpreg = RCC->CFGR2;
    tmpreg &= ~CFGR2_PLL3MUL;      //Clear PLL3Mul[3:0] bits
    tmpreg |= RCC_PLL3Mul;        //Set the PLL3 configuration bits
    RCC->CFGR2 = tmpreg;        //Store the new value
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_PLL3Cmd(FunctionalState NewState)
{
  *(__IO uint32_t *) CR_PLL3ON_BB = (uint32_t)NewState;
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_PREDIV1Config(uint32_t RCC_PREDIV1_Source, uint32_t RCC_PREDIV1_Div)
{
    uint32_t tmpreg = 0;

    tmpreg = RCC->CFGR2;
    tmpreg &= ~(CFGR2_PREDIV1 | CFGR2_PREDIV1SRC);    //Clear PREDIV1[3:0] and PREDIV1SRC bits
    tmpreg |= RCC_PREDIV1_Source | RCC_PREDIV1_Div ;  //Set the PREDIV1 clock source and division factor
    RCC->CFGR2 = tmpreg;                //Store the new value
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLMul)
{
    uint32_t tmpreg = 0;

    tmpreg = RCC->CFGR;
    tmpreg &= CFGR_PLL_Mask;        //Clear PLLSRC, PLLXTPRE and PLLMUL[3:0] bits
    tmpreg |= RCC_PLLSource | RCC_PLLMul;  //Set the PLL configuration bits
    RCC->CFGR = tmpreg;            //Store the new value
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_I2S2CLKConfig(uint32_t RCC_I2S2CLKSource)
{
  *(__IO uint32_t *) CFGR2_I2S2SRC_BB = RCC_I2S2CLKSource;
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_I2S3CLKConfig(uint32_t RCC_I2S3CLKSource)
{
  *(__IO uint32_t *) CFGR2_I2S3SRC_BB = RCC_I2S3CLKSource;
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_PLLCmd(FunctionalState NewState)
{
  *(__IO uint32_t *) CR_PLLON_BB = (uint32_t)NewState;
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource)
{
    uint32_t tmpreg = 0;

    tmpreg = RCC->CFGR;
    tmpreg &= CFGR_SW_Mask;          //Clear SW[1:0] bits
    tmpreg |= RCC_SYSCLKSource;        //Set SW[1:0] bits according to RCC_SYSCLKSource value
    RCC->CFGR = tmpreg;            //Store the new value
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
uint8_t RCC_GetSYSCLKSource(void)
{
  return ((uint8_t)(RCC->CFGR & CFGR_SWS_Mask));
}  
     
/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    RCC->APB2ENR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}  
  
/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    RCC->AHBENR |= RCC_AHBPeriph;
  }
  else
  {
    RCC->AHBENR &= ~RCC_AHBPeriph;
  }
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    RCC->APB1ENR |= RCC_APB1Periph;
  }
  else
  {
    RCC->APB1ENR &= ~RCC_APB1Periph;
  }
}  
  
/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    RCC->APB1RSTR |= RCC_APB1Periph;
  }
  else
  {
    RCC->APB1RSTR &= ~RCC_APB1Periph;
  }
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
  uint32_t tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

#ifdef  STM32F10X_CL
  uint32_t prediv1source = 0, prediv1factor = 0, prediv2factor = 0, pll2mull = 0;
#endif /* STM32F10X_CL */
    
  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & CFGR_SWS_Mask;
  
  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock */
      RCC_Clocks->SYSCLK_Frequency = HSI_Value;
      break;
    case 0x04:  /* HSE used as system clock */
      RCC_Clocks->SYSCLK_Frequency = HSE_Value;
      break;
    case 0x08:  /* PLL used as system clock */

      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmull = RCC->CFGR & CFGR_PLLMull_Mask;
      pllsource = RCC->CFGR & CFGR_PLLSRC_Mask;
      
#ifndef STM32F10X_CL      
      pllmull = ( pllmull >> 18) + 2;
      
      if (pllsource == 0x00)
      {/* HSI oscillator clock divided by 2 selected as PLL clock entry */
        RCC_Clocks->SYSCLK_Frequency = (HSI_Value >> 1) * pllmull;
      }
      else
      {/* HSE selected as PLL clock entry */
        if ((RCC->CFGR & CFGR_PLLXTPRE_Mask) != (uint32_t)RESET)
        {/* HSE oscillator clock divided by 2 */
          RCC_Clocks->SYSCLK_Frequency = (HSE_Value >> 1) * pllmull;
        }
        else
        {
          RCC_Clocks->SYSCLK_Frequency = HSE_Value * pllmull;
        }
      }
#else
      pllmull = pllmull >> 18;
      
      if (pllmull != 0x0D)
      {
         pllmull += 2;
      }
      else
      { /* PLL multiplication factor = PLL input clock * 6.5 */
        pllmull = 13 / 2; 
      }
            
      if (pllsource == 0x00)
      {/* HSI oscillator clock divided by 2 selected as PLL clock entry */
        RCC_Clocks->SYSCLK_Frequency = (HSI_Value >> 1) * pllmull;
      }
      else
      {/* PREDIV1 selected as PLL clock entry */
        
        /* Get PREDIV1 clock source and division factor */
        prediv1source = RCC->CFGR2 & CFGR2_PREDIV1SRC;
        prediv1factor = (RCC->CFGR2 & CFGR2_PREDIV1) + 1;
        
        if (prediv1source == 0)
        { /* HSE oscillator clock selected as PREDIV1 clock entry */
          RCC_Clocks->SYSCLK_Frequency = (HSE_Value / prediv1factor) * pllmull;          
        }
        else
        {/* PLL2 clock selected as PREDIV1 clock entry */
          
          /* Get PREDIV2 division factor and PLL2 multiplication factor */
          prediv2factor = ((RCC->CFGR2 & CFGR2_PREDIV2) >> 4) + 1;
          pll2mull = ((RCC->CFGR2 & CFGR2_PLL2MUL) >> 8 ) + 2; 
          RCC_Clocks->SYSCLK_Frequency = (((HSE_Value / prediv2factor) * pll2mull) / prediv1factor) * pllmull;                         
        }
      }
#endif /* STM32F10X_CL */ 
      break;

    default:
      RCC_Clocks->SYSCLK_Frequency = HSI_Value;
      break;
  }

  /* Compute HCLK, PCLK1, PCLK2 and ADCCLK clocks frequencies ----------------*/
  /* Get HCLK prescaler */
  tmp = RCC->CFGR & CFGR_HPRE_Set_Mask;
  tmp = tmp >> 4;
  presc = APBAHBPrescTable[tmp];
  /* HCLK clock frequency */
  RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;
  /* Get PCLK1 prescaler */
  tmp = RCC->CFGR & CFGR_PPRE1_Set_Mask;
  tmp = tmp >> 8;
  presc = APBAHBPrescTable[tmp];
  /* PCLK1 clock frequency */
  RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
  /* Get PCLK2 prescaler */
  tmp = RCC->CFGR & CFGR_PPRE2_Set_Mask;
  tmp = tmp >> 11;
  presc = APBAHBPrescTable[tmp];
  /* PCLK2 clock frequency */
  RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
  /* Get ADCCLK prescaler */
  tmp = RCC->CFGR & CFGR_ADCPRE_Set_Mask;
  tmp = tmp >> 14;
  presc = ADCPrescTable[tmp];
  /* ADCCLK clock frequency */
  RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency / presc;
}  

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/  
void Set_SysCLK(char SelSysCLK)                  //Systemclock auf HSI,HSE oder PLL setzen
{
  if(SelSysCLK==SysCLK_HSI)                   //HSI ausgewählt?
  {
      RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);          //Select HSI (8MHz) as system clock source
       while(RCC_GetSYSCLKSource() != 0x00)          //Wait till HSI is used as system clock source
      {}
  } 
  else if(SelSysCLK==SysCLK_HSE)                //HSE ausgewählt?
  {
      RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);          //Select HSE (25MHz) as system clock source
       while(RCC_GetSYSCLKSource() != 0x04)          //Wait till HSE is used as system clock source
      {}
  }
  else if(SelSysCLK==SysCLK_PLL)                //PLL ausgewählt?
  {
      RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        //Select PLL as system clock source
       while(RCC_GetSYSCLKSource() != 0x08)          //Wait till PLL is used as system clock source
      {}
  }
}

