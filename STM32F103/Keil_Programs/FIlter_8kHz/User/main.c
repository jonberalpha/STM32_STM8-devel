/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:                                                                 */
/* Autor:     Kimmel                                                          */
/* Version:   V1.40                                                           */
/* Date:                                                                      */
/* Description:                                                               */
/******************************************************************************/
/* History: V0.00  creation                                                   */
/*          V1.0   02.07.2013 Alexander Kimmel                                */
/*          V1.1   5.1.2013 Fixpoint (ohne Filter)                            */
/*          V1.2   10.1.2013 Fixpoint (Filter gefixt)                         */
/*          V1.4   10.1.2013 Fixpoint (Filter gefixt)                         */
/******************************************************************************/
#include "armv20_std.h"
#include "rcc_lib.h"
#include "i2s_lib.h"
#include "dma_lib.h"
#include "adc_dac_lib.h"
#include "arm_math.h"


/*------------------------------ Function Prototypes -------------------------*/
static void Start_Audio_Processing(void);
static void SetSystem(void);
static void NVIC_init(char position, char priority); 

extern q15_t Audio_IN[], Audio_OUT[];

/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main (void)
{
  char x=0;                                                          

  SetSystem();        //hw_config.h          //RCC-DeInit, HSE-config, PLL-Config,Sysclock=PLL,DMA-Init,etc                                  
  //Set_SysCLK(SysCLK_HSI);                  //SysCLK=HSI = 8MHz
  NVIC_init(14,1);

  init_leds_switches();                      //Leds und Schalter initialisieren
  set_leds(0);                              //Leds auf 0 setzen    
  
  Start_Audio_Processing();                  //Audioadapter init
  
  while(1)
  {  
    set_leds(x);
    x++;  
    wait_ms(1000);
  }  
}      

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static void Start_Audio_Processing(void)
{      
  DAC_init();         //DAC wird initialisiert
  ADC_init();        //ADC wird initialisiert
  
  I2S_Config();        //Configure the I2S peripheral
  
  Set_SysCLK(SysCLK_PLL);   //SysClk=PLL=72MHz
  
  I2S_Cmd(SPI2,ENABLE);   //I2S2 - enable
  I2S_Cmd(SPI3,ENABLE);  //I2S3 - enable
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static void SetSystem(void)
{
  int status=0;     
  DMA_InitTypeDef DMA_InitStructure;                     //Init-Struktur-Definition für DMA
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -------------------------*/  
  RCC_DeInit();                              //RCC system reset(for debug purpose)
  RCC_HSEConfig();                          //Enable HSE
  status=RCC_WaitForHSEStartUp();                    //Wait till HSE is ready

  if(status==1)
  {
    FLASH_PrefetchBufferCmd();                    //Enable Prefetch Buffer
    FLASH_SetLatency();                        //Flash 2 wait state
    RCC_HCLKConfig();                        //HCLK = SYSCLK
    RCC_PCLK2Config();                        //PCLK2 = HCLK
    RCC_PCLK1Config();                        //PCLK1 = HCLK/2

    /* Configure PLLs---------------------------------------------------*/
    //PLL2 configuration: PLL2CLK = (HSE / 9) * 13 = 36.111 MHz
    RCC_PREDIV2Config(RCC_PREDIV2_Div9);               //HSE / 9 = PRE 
    RCC_PLL2Config(RCC_PLL2Mul_13);                 //PRE * 13 = PLL2
    RCC_PLL2Cmd(ENABLE);                      //Enable PLL2
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == 0);         //Wait till PLL2 is ready

    //PPL3 configuration: PLL3CLK = (HSE / 9) * 14 = PLL3_VCO = 77.777 MHz
    RCC_PLL3Config(RCC_PLL3Mul_14);                  //PRE*14
    RCC_PLL3Cmd(ENABLE);                       //Enable PLL3
    while (RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == 0);        //Wait till PLL3 is ready
     
    //PLL configuration: PLLCLK = (PLL2 / 4) * 8 = 72.222 MHz
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div4);  //PLL2 / 4 = PRE1
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_8);        //PRE1 * 8 = PLL

    RCC_I2S2CLKConfig(RCC_I2S2CLKSource_PLL3_VCO);           //Configure I2S clock source: PLL3 VCO clock
    RCC_I2S3CLKConfig(RCC_I2S3CLKSource_PLL3_VCO);           //Configure I2S clock source: PLL3 VCO clock
  
    RCC_PLLCmd(ENABLE);                        //Enable PLL
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == 0);          //Wait till PLL is ready
    
  }
  else
  {   
    while(1);//RP jetzt gehts schief,weil Pin13 und 14 einen KS hatten
  }
  
  // Enable GPIOx (x = A, B, C, D, E, F, G), SPI1 clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | 
  RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | 
  RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |
  RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO  |
  RCC_APB2Periph_SPI1,ENABLE);
            
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);              //Enable the DMA2 Clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);              //Enable the DMA1 clock     
  SysTick_Config(SystemCoreClock / 100); //SystemFrequency
     
  DMA_DeInit(DMA1_Channel4);                            //DMA1_Channel4 deinit
  DMA_InitStructure.DMA_PeripheralBaseAddr = SPI2_DR_Address;          //Peripherie-Adresse: SPI2-Datenregister
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&Audio_IN[0];        //Veraendert, theoretisch auf jeder Adresse moeglich

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;              //Richtung: Peripherie=Quelle
  DMA_InitStructure.DMA_BufferSize = 2;                    //verändert; Holt 2 Werte 
    
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;      //Peripherie-Inkrement: disable
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;            //Speicher-Inkrementierun: enable
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //Datengröße: Half-Word: 16b
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;      //Datengröße: Half-Word: 16b
  
   DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                //Circular-Mode
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                //Normal-Mode , bis Buffersize fertig ==> um neuen Durchgang zu starten, 
                                          //muss man in das CNDTR register den jeweiligen Wert hineinschreiben
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;              //Priorität: High
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                //Memory to Memory: Disable
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);                //Initialisierung

  DMA1_Channel4->CCR |= 0x2;                          //Transmition-complete: Interrupt
  DMA_ClearITPendingBit(DMA1_FLAG_TC4);                      //Interrupt-Bit löschen

  DMA_DeInit(DMA2_Channel2);                             //DMA2_Channel2 deinit
  DMA_InitStructure.DMA_PeripheralBaseAddr = SPI3_DR_Address;          //Peripherie-Adresse: SPI3-Datenregister
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&Audio_OUT[0];        //Veraendert
 
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;              //Richtung: Peripherie=Ziel
  DMA_InitStructure.DMA_BufferSize = 2;                    //veraendert
    
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;      //Peripherie-Inkrement: disable
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;            //Speicher-Inkrementierung: enable
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //Datengröße: Half-Word: 16b
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;      //Datengröße: Half-Word: 16b
    
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;              //Circular-Mode
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                //Normal-Mode
    
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;              //Priorität: High
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                //Memory to Memory: Disable
  DMA_Init(DMA2_Channel2, &DMA_InitStructure);                //Initialisierung
  
  DMA2_Channel2->CCR |= 0x2;                          //Transmition-complete: Interrupt
  DMA_ClearITPendingBit(DMA2_FLAG_TC2);                      //Interrupt-Bit löschen

   DMA_Cmd(DMA2_Channel2, ENABLE);                        //Enablen: DMA2_CH2
  DMA_Cmd(DMA1_Channel4, ENABLE);                        //Enablen: DMA1_CH4         
            
}

/******************************************************************************/
/*                   NVIC_init(char position, char priority)                  */
/* Funktion:                                                                  */
/*   Übernimmt die vollständige initialisierung eines Interrupts  im Nested   */
/*   vectored Interrupt controller (Priorität setzen, Auslösen verhindern,    */
/*   Interrupt enablen)                                                       */
/* Übergabeparameter: "position" = 0-67 (Nummer des Interrupts)               */
/*                    "priority": 0-15 (Priorität des Interrupts)             */
/******************************************************************************/
static void NVIC_init(char position, char priority) 
{  
  NVIC->IP[position]=(priority<<4);   //Setzen der Interrupt Priorität
  NVIC->ICPR[position/32] |= (0x01<<(position%32));  //Verhindert, dass das Interrupt auslöst sobald es enabled wird
  NVIC->ISER[position/32] |=  (0x01<<(position%32));    //Enable interrupt
} 
