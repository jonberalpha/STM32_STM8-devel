#define __I2S_LIB_MOD
#include "stm32f10x.h"
#include "gpio_lib.h"
#include "i2s_lib.h"
#include "rcc_lib.h"


/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void I2S_Config(void)
{
  char tempreg=0;
  I2S_InitTypeDef I2S_InitStructure;

  /*I2S3*/
  // I2S3 SD, CK, MCLK and WS pins configuration
  init_port(GPIOC,10,10,AFO_pushpull,frequency_50MHz);
  init_port(GPIOC,12,12,AFO_pushpull,frequency_50MHz);
  init_port(GPIOA,4,4,AFO_pushpull,frequency_50MHz);
  init_port(GPIOC,7,7,AFO_pushpull,frequency_50MHz);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);         //Enable I2S3 APB1 clock
  SPI3_I2S_DeInit();                            //Deinitialize SPI3 peripheral


  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;            //Modus: Master-Transmitter       <--richtig
  //I2S_InitStructure.I2S_Mode = I2S_Mode_SlaveTx;            //Modus: Master-Transmitter         //Slave ? !!!


  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;        //Standard: I2S-Phillips-Standard
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;        //Datenformat: 16b
   I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;      //MCLK: disable
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_8k;         //Sample-Frequenz: 8kHz
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;                 //Clock-Polarity: Low
  I2S_Init(SPI3, &I2S_InitStructure);                  //Initialisierung

  /*I2S2*/
    // I2S2 SD, CK, MLK and WS pins configuration

  //init_port(GPIOB,13,13,AFO_pushpull,frequency_50MHz);          //<-- so richtig?
  //init_port(GPIOB,12,12,AFO_pushpull,frequency_50MHz);

  init_port(GPIOB,12,13,AFO_pushpull,frequency_50MHz);    ///?????
  init_port(GPIOB,15,15,AFO_pushpull,frequency_50MHz);
  init_port(GPIOC,6,6,AFO_pushpull,frequency_50MHz);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);           //Enable I2S2 APB1 clock
  SPI2_I2S_DeInit();                            //Deinitialize SPI3 peripheral


  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterRx;            //Modus: Master-Receiver        //<--vorher
  //I2S_InitStructure.I2S_Mode = I2S_Mode_SlaveRx;            //Modus: Master-Receiver


  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;        //Standard: I2S-Phillips-Standard
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;        //Datenformat: 16b
   I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;      //MCLK: enable
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_8k;         //Sample-Frequenz: 8kHz
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;                 //Clock-Polarity: Low
  I2S_Init(SPI2, &I2S_InitStructure);                  //Initialisierung


  //RXDMA setzen --> DMA holt Daten wenn DR voll
  tempreg=SPI2->CR2;
  tempreg|=0x0001;
  SPI2->CR2=tempreg;

  //TXDMA setzen --> DMA bringt Daten wenn DR leer
  tempreg=SPI3->CR2;
  tempreg|=0x0002;
  SPI3->CR2=tempreg;
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void I2S_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Enable the selected SPI peripheral (in I2S mode) */
    SPIx->I2SCFGR |= I2SCFGR_I2SE_Set;
  }
  else
  {
    /* Disable the selected SPI peripheral (in I2S mode) */
    SPIx->I2SCFGR &= I2SCFGR_I2SE_Reset;
  }
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void SPI3_I2S_DeInit(void)
{
  // Enable SPI3 reset state
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);
  // Release SPI3 from reset state
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void SPI2_I2S_DeInit(void)
{
  // Enable SPI3 reset state
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);
  // Release SPI3 from reset state
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void I2S_Init(SPI_TypeDef* SPIx, I2S_InitTypeDef* I2S_InitStruct)
{
  uint16_t tmpreg = 0, i2sdiv = 2, i2sodd = 0, packetlength = 1;
  uint32_t tmp = 0;
  RCC_ClocksTypeDef RCC_Clocks;
  uint32_t sourceclock = 0;


/*----------------------- SPIx I2SCFGR & I2SPR Configuration -----------------*/
  /* Clear I2SMOD, I2SE, I2SCFG, PCMSYNC, I2SSTD, CKPOL, DATLEN and CHLEN bits */
  SPIx->I2SCFGR &= I2SCFGR_CLEAR_Mask;
  SPIx->I2SPR = 0x0002;

  /* Get the I2SCFGR register value */
  tmpreg = SPIx->I2SCFGR;

  /* If the default value has to be written, reinitialize i2sdiv and i2sodd*/
  if(I2S_InitStruct->I2S_AudioFreq == I2S_AudioFreq_Default)
  {
    i2sodd = (uint16_t)0;
    i2sdiv = (uint16_t)2;
  }
  /* If the requested audio frequency is not the default, compute the prescaler */
  else
  {
    /* Check the frame length (For the Prescaler computing) */
    if(I2S_InitStruct->I2S_DataFormat == I2S_DataFormat_16b)
    {
      /* Packet length is 16 bits */
      packetlength = 1;
    }
    else
    {
      /* Packet length is 32 bits */
      packetlength = 2;
    }

    /* Get the I2S clock source mask depending on the peripheral number */
    if(((uint32_t)SPIx) == SPI3_BASE)
    {
      /* The mask is relative to I2S2 */
      tmp = I2S2_CLOCK_SRC;
    }
    else
    {
      /* The mask is relative to I2S3 */
      tmp = I2S3_CLOCK_SRC;
    }

    /* Check the I2S clock source configuration depending on the Device:
       Only Connectivity line devices have the PLL3 VCO clock */
    if((RCC->CFGR2 & tmp) != 0)
    {
      /* Get the configuration bits of RCC PLL3 multiplier */
      tmp = (uint32_t)((RCC->CFGR2 & I2S_MUL_MASK) >> 12);

      /* Get the value of the PLL3 multiplier */
      if((tmp > 5) && (tmp < 15))
      {
        /* Multplier is between 8 and 14 (value 15 is forbidden) */
        tmp += 2;
      }
      else
      {
        if (tmp == 15)
        {
          /* Multiplier is 20 */
          tmp = 20;
        }
      }
      /* Get the PREDIV2 value */
      sourceclock = (uint32_t)(((RCC->CFGR2 & I2S_DIV_MASK) >> 4) + 1);

      /* Calculate the Source Clock frequency based on PLL3 and PREDIV2 values */
      sourceclock = (uint32_t) ((HSE_Value / sourceclock) * tmp * 2);
    }
    else
    {
      /* I2S Clock source is System clock: Get System Clock frequency */
      RCC_GetClocksFreq(&RCC_Clocks);

      /* Get the source clock value: based on System Clock value */
      sourceclock = RCC_Clocks.SYSCLK_Frequency;
    }

    /* Compute the Real divider depending on the MCLK output state with a flaoting point */
    if(I2S_InitStruct->I2S_MCLKOutput == I2S_MCLKOutput_Enable)
    {
      /* MCLK output is enabled */
      tmp = (uint16_t)(((((sourceclock / 256) * 10) / I2S_InitStruct->I2S_AudioFreq)) + 5);
    }
    else
    {
      /* MCLK output is disabled */
      tmp = (uint16_t)(((((sourceclock / (32 * packetlength)) *10 ) / I2S_InitStruct->I2S_AudioFreq)) + 5);
    }

    /* Remove the flaoting point */
    tmp = tmp / 10;

    /* Check the parity of the divider */
    i2sodd = (uint16_t)(tmp & (uint16_t)0x0001);

    /* Compute the i2sdiv prescaler */
    i2sdiv = (uint16_t)((tmp - i2sodd) / 2);

    /* Get the Mask for the Odd bit (SPI_I2SPR[8]) register */
    i2sodd = (uint16_t) (i2sodd << 8);
  }

  /* Test if the divider is 1 or 0 or greater than 0xFF */
  if ((i2sdiv < 2) || (i2sdiv > 0xFF))
  {
    /* Set the default values */
    i2sdiv = 2;
    i2sodd = 0;
  }

  /* Write to SPIx I2SPR register the computed value */
  SPIx->I2SPR = (uint16_t)(i2sdiv | (uint16_t)(i2sodd | (uint16_t)I2S_InitStruct->I2S_MCLKOutput));

  /* Configure the I2S with the SPI_InitStruct values */
  tmpreg |= (uint16_t)(I2S_Mode_Select | (uint16_t)(I2S_InitStruct->I2S_Mode | \
                  (uint16_t)(I2S_InitStruct->I2S_Standard | (uint16_t)(I2S_InitStruct->I2S_DataFormat | \
                  (uint16_t)I2S_InitStruct->I2S_CPOL))));

  /* Write to SPIx I2SCFGR */
  SPIx->I2SCFGR = tmpreg;
}

