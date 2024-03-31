/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   Sensor.c                                                      */
/* Autor:     Josef Reisinger, Gerald Zottl                                   */
/* Version:   V1.10                                                           */
/* Date:      21/01/2018                                                      */
/* Description: Einlesen der Sensorwerte für Digitalen Regler                 */
/*              (Istwert, Sollwert) mittels ADC und DMA Controller            */  
/******************************************************************************/
/* History:   V1.00  REJ: creation                                            */
/*            V1.10  ZOG: add 4 morce ADC inputs                              */
/******************************************************************************/
/*  Sensors:                                                                  */
/*  PB1     -> ADC1_IN9  Analog Input for desired Value (Sollwert)            */
/*  PC4     -> ADC1_IN14 Analog Input for measured Value (Istwert)            */
/*  PA0     -> ADC1_IN0 Analog Input                                          */
/*  PA1     -> ADC1_IN1 Analog Input                                          */
/*  PA2     -> ADC1_IN2 Analog Input                                          */
/*  PA3     -> ADC1_IN3 Analog Input                                          */
/******************************************************************************/
#define SENSOREN_MOD
#include <sensoren.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_dma.h>

#define NR_OF_ACTIVE_ADCs 6 // ZOG: The number of active ADC channels.

/*--------------------------- Modul Globale Variablen ------------------------------------*/
//static __IO uint16_t ADC1ConvertedValues[2];  //  Array für ADC Werte
static __IO uint16_t ADC1ConvertedValues[NR_OF_ACTIVE_ADCs];  // ZOG: Array für ADC Werte

/*----------------------------------------------- Static Function Prototypes --------------*/
static void NVIC_init(char position, char priority);

/*------------------------------ Definitions --------------------------------*/
#define ADC1_DR_ADDRESS    ((uint32_t)0x4001244C)

/*---------------------------------------------------------------------------*/
/*       Initialisiert DMA Controler für ADC um Sensorwerte zu kopieren      */
/*---------------------------------------------------------------------------*/
void DMA_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  /* Initialize the DMA1 Channel1 according to the DMA_InitStructure members */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);              // REJ: Enable the DMA1 clock     
  DMA_DeInit(DMA1_Channel1);                                      //REJ: DMA1_Channel1 deinit
  
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;          //REJ: Source Address for copy process 
  DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)&ADC1ConvertedValues;  // REJ: Destination fpr copy process
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  //  DMA_InitStructure.DMA_BufferSize = 2;                           // copy  two 16bit ADC Values to Memory
  DMA_InitStructure.DMA_BufferSize = NR_OF_ACTIVE_ADCs;   //ZOG: copy 16bit ADC Values to Memory for all active channels
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;      // Holt 16Bit Wert
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //Better for SCAN and CONTINOUS ADC modes
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  DMA1_Channel1->CCR |= 0x2;                          //REJ:Transmition-complete --> Interrupt
  DMA_ClearITPendingBit(DMA1_FLAG_TC1);                //REJ:Interrupt-Bit löschen

  /* Enable DMA1 Channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  NVIC_init(11,1);      // REJ: Enable Interrupt DMA1 Channel1 Interrupt (Serves ADC1)

}

/*---------------------------------------------------------------------------*/
/*                    Initialisiert Sensoren für Regler                      */
/*---------------------------------------------------------------------------*/
void SensorInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;  

  RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // Vorteiler für ADC-Takt
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE); // ZOG GPIOA added
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  // ZOG: GPIOA Pins for ADC inputs: PA0=ADC12_IN0; PA1=ADC12_IN1; PA2=ADC12_IN2; PA3=ADC12_IN3
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;    // Analog In für ADCs um analoge Werte einzulesen
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Pin für ADC Eingang:  PB1=ADC12_IN9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;    // Analog In für ADCs um analoge Werte einzulesen
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Pins für ADC Eingang: PC4=ADC12_IN14
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;    // Analog In für ADCs um analoge Werte einzulesen
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
    
  // REJ: ADC1 für Sensoren konfigurieren (2 analoge Inputs)
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
//  ADC_InitStructure.ADC_NbrOfChannel = 2;       // 2 Channels: ADC12_IN9,  ADC12_IN14
  ADC_InitStructure.ADC_NbrOfChannel = NR_OF_ACTIVE_ADCs;       // ZOG: number of all active channels
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;       //REJ: Enable Scan Mode -> Multiple Channel Input possible
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_9,  1, ADC_SampleTime_55Cycles5);  // ADC1 für PB1 (Channel 9)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 2, ADC_SampleTime_55Cycles5);  // ADC1 für PC4 (Channel 14)
  // ZOG added
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0,  3, ADC_SampleTime_55Cycles5);  // ADC1 für PA0 (Channel 0)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1,  4, ADC_SampleTime_55Cycles5);  // ADC1 für PA1 (Channel 1)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2,  5, ADC_SampleTime_55Cycles5);  // ADC1 für PA2 (Channel 2)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3,  6, ADC_SampleTime_55Cycles5);  // ADC1 für PA3 (Channel 3)

  /* Enable ADC1 DMA transfer */
  ADC_DMACmd(ADC1, ENABLE);

  ADC_Cmd(ADC1, ENABLE);  // ADC aktivieren

  ADC_ResetCalibration(ADC1);          // Kalibrierungsvorgang der ADC1
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));

  ADC_SoftwareStartConvCmd(ADC1, ENABLE);    // triggert die erste Wandlung der beiden ADCs

}

/*---------------------------------------------------------------------------*/
/*             Get Desired Value                                             */
/*---------------------------------------------------------------------------*/
int16_t Get_Desired_Value(void)
{
  return(ADC1ConvertedValues[1]);

}

/*---------------------------------------------------------------------------*/
/*            Get Measured Value                                             */
/*---------------------------------------------------------------------------*/
int16_t Get_Measured_Value(void)
{
  return(ADC1ConvertedValues[0]);

}

// ZOG: begin
//-----------------------------------------------------------------------------
// Get ADC values
//-----------------------------------------------------------------------------
int16_t Get_ADC_PB1(void)
{
  return(ADC1ConvertedValues[0]);
}
int16_t Get_ADC_PC4(void)
{
  return(ADC1ConvertedValues[1]);
}
int16_t Get_ADC_PA0(void)
{
  return(ADC1ConvertedValues[2]);
}
int16_t Get_ADC_PA1(void)
{
  return(ADC1ConvertedValues[3]);
}
int16_t Get_ADC_PA2(void)
{
  return(ADC1ConvertedValues[4]);
}
int16_t Get_ADC_PA3(void)
{
  return(ADC1ConvertedValues[5]);
}
// ZOG: end


/******************************************************************************/
/*                   NVIC_init(char position, char priority)            */ 
/* Funktion:                                                                  */    
/*   Übernimmt die vollständige initialisierung eines Interrupts  im Nested   */
/*   vectored Interrupt controller (Priorität setzen, Auslösen verhindern,    */
/*   Interrupt enablen)                                                       */
/* Übergabeparameter: "position" = 0-67 (Nummer des Interrupts)               */
/*                    "priority": 0-15 (Priorität des Interrupts)          */
/******************************************************************************/
static void NVIC_init(char position, char priority) 
{  
  NVIC->IP[position]=(priority<<4);   //Setzen der Interrupt Priorität
  NVIC->ICPR[position/32] |= (0x01<<(position%32));  //Verhindert, dass das Interrupt auslöst sobald es enabled wird
  NVIC->ISER[position/32] |= (0x01<<(position%32));    //Enable interrupt
} 
