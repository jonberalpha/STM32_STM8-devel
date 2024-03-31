#ifndef  __RCC_LIB_H     //keine Mehrfach definition
#define  __RCC_LIB_H

#include "stm32f10x.h"

/********************************************************************************************/
/*    Defines                                        */
/********************************************************************************************/         
/**********    RCC    **********/
/* Flags + Masks */
//CR register bit mask
#define CR_HSEBYP_Reset             ((uint32_t)0xFFFBFFFF)  //HSEBYP = 0 für external Resonator (Quarz, etc)
#define CR_HSEBYP_Set               ((uint32_t)0x00040000)  //HSEBYP = 1 für external Clock Source (Oszillator)      
#define CR_HSEON_Reset              ((uint32_t)0xFFFEFFFF)    //HSEON = 0 --> HSE nicht aktiv
#define CR_HSEON_Set                ((uint32_t)0x00010000)  //HSEON = 1 --> HSE  kann verwendet werden
#define CR_HSITRIM_Mask             ((uint32_t)0xFFFFFF07)
//CFGR register bit mask
#define CFGR_PLL_Mask              ((uint32_t)0xFFC2FFFF)
#define CFGR_PLLMull_Mask           ((uint32_t)0x003C0000)
#define CFGR_PLLSRC_Mask            ((uint32_t)0x00010000)
#define CFGR_PLLXTPRE_Mask          ((uint32_t)0x00020000)
#define CFGR_SWS_Mask               ((uint32_t)0x0000000C)
#define CFGR_SW_Mask                ((uint32_t)0xFFFFFFFC)
#define CFGR_HPRE_Reset_Mask        ((uint32_t)0xFFFFFF0F)
#define CFGR_HPRE_Set_Mask          ((uint32_t)0x000000F0)
#define CFGR_PPRE1_Reset_Mask       ((uint32_t)0xFFFFF8FF)
#define CFGR_PPRE1_Set_Mask         ((uint32_t)0x00000700)
#define CFGR_PPRE2_Reset_Mask       ((uint32_t)0xFFFFC7FF)
#define CFGR_PPRE2_Set_Mask         ((uint32_t)0x00003800)
#define CFGR_ADCPRE_Reset_Mask      ((uint32_t)0xFFFF3FFF)
#define CFGR_ADCPRE_Set_Mask        ((uint32_t)0x0000C000)
//CFGR2 register bit mask
#define CFGR2_PREDIV1SRC           ((uint32_t)0x00010000)
#define CFGR2_PREDIV1              ((uint32_t)0x0000000F)
#define CFGR2_PREDIV2              ((uint32_t)0x000000F0)
#define CFGR2_PLL2MUL              ((uint32_t)0x00000F00)
#define CFGR2_PLL3MUL              ((uint32_t)0x0000F000)
// RCC Flag Mask
#define FLAG_Mask                   ((uint8_t)0x1F)
// Typical Value of the HSI in Hz
#ifndef HSI_Value
 #define HSI_Value                   ((uint32_t)8000000)    //High Speed Intern Oszillator Frequency
#endif /* HSI_Value */
// Time out for HSE start up         
#ifndef HSEStartUp_TimeOut
 #define HSEStartUp_TimeOut          ((uint16_t)0x0500)
#endif /* HSEStartUp_TimeOut */     
//PLL_entry_clock_source 
#define RCC_PLLSource_HSI_Div2       ((uint32_t)0x00000000)
#define RCC_PLLSource_PREDIV1       ((uint32_t)0x00010000) 
//PLL_multiplication_factor   
//#define RCC_PLLMul_2               ((uint32_t)0x00000000)
//#define RCC_PLLMul_3                ((uint32_t)0x00040000)
#define RCC_PLLMul_4                ((uint32_t)0x00080000)
#define RCC_PLLMul_5                 ((uint32_t)0x000C0000)
#define RCC_PLLMul_6                 ((uint32_t)0x00100000)
#define RCC_PLLMul_7               ((uint32_t)0x00140000)
#define RCC_PLLMul_8                 ((uint32_t)0x00180000)
#define RCC_PLLMul_9                ((uint32_t)0x001C0000)
//#define RCC_PLLMul_10               ((uint32_t)0x00200000)
//#define RCC_PLLMul_11               ((uint32_t)0x00240000)
//#define RCC_PLLMul_12              ((uint32_t)0x00280000)
//#define RCC_PLLMul_13                ((uint32_t)0x002C0000)
//#define RCC_PLLMul_14                ((uint32_t)0x00300000)
//#define RCC_PLLMul_15                ((uint32_t)0x00340000)
//#define RCC_PLLMul_16                ((uint32_t)0x00380000)
#define RCC_PLLMul_6_5                ((uint32_t)0x00340000)  
//PREDIV1_division_factor   
#define RCC_PREDIV1_Div1             ((uint32_t)0x00000000)
#define RCC_PREDIV1_Div2            ((uint32_t)0x00000001)
#define RCC_PREDIV1_Div3            ((uint32_t)0x00000002)
#define RCC_PREDIV1_Div4            ((uint32_t)0x00000003)
#define RCC_PREDIV1_Div5            ((uint32_t)0x00000004)
#define RCC_PREDIV1_Div6            ((uint32_t)0x00000005)
#define RCC_PREDIV1_Div7            ((uint32_t)0x00000006)
#define RCC_PREDIV1_Div8            ((uint32_t)0x00000007)
#define RCC_PREDIV1_Div9            ((uint32_t)0x00000008)
#define RCC_PREDIV1_Div10           ((uint32_t)0x00000009)
#define RCC_PREDIV1_Div11           ((uint32_t)0x0000000A)
#define RCC_PREDIV1_Div12           ((uint32_t)0x0000000B)
#define RCC_PREDIV1_Div13           ((uint32_t)0x0000000C)
#define RCC_PREDIV1_Div14           ((uint32_t)0x0000000D)
#define RCC_PREDIV1_Div15           ((uint32_t)0x0000000E)
#define RCC_PREDIV1_Div16           ((uint32_t)0x0000000F)
//PREDIV1_clock_source
#define RCC_PREDIV1_Source_HSE       ((uint32_t)0x00000000) 
#define RCC_PREDIV1_Source_PLL2      ((uint32_t)0x00010000)
//PREDIV2_division_factor
#define RCC_PREDIV2_Div1           ((uint32_t)0x00000000)
#define RCC_PREDIV2_Div2            ((uint32_t)0x00000010)
#define RCC_PREDIV2_Div3           ((uint32_t)0x00000020)
#define RCC_PREDIV2_Div4            ((uint32_t)0x00000030)
#define RCC_PREDIV2_Div5            ((uint32_t)0x00000040)
#define RCC_PREDIV2_Div6            ((uint32_t)0x00000050)
#define RCC_PREDIV2_Div7           ((uint32_t)0x00000060)
#define RCC_PREDIV2_Div8           ((uint32_t)0x00000070)
#define RCC_PREDIV2_Div9            ((uint32_t)0x00000080)
#define RCC_PREDIV2_Div10           ((uint32_t)0x00000090)
#define RCC_PREDIV2_Div11          ((uint32_t)0x000000A0)
#define RCC_PREDIV2_Div12          ((uint32_t)0x000000B0)
#define RCC_PREDIV2_Div13          ((uint32_t)0x000000C0)
#define RCC_PREDIV2_Div14          ((uint32_t)0x000000D0)
#define RCC_PREDIV2_Div15          ((uint32_t)0x000000E0)
#define RCC_PREDIV2_Div16          ((uint32_t)0x000000F0)   
//PLL2_multiplication_factor
#define RCC_PLL2Mul_8               ((uint32_t)0x00000600)
#define RCC_PLL2Mul_9                ((uint32_t)0x00000700)
#define RCC_PLL2Mul_10             ((uint32_t)0x00000800)
#define RCC_PLL2Mul_11              ((uint32_t)0x00000900)
#define RCC_PLL2Mul_12               ((uint32_t)0x00000A00)
#define RCC_PLL2Mul_13             ((uint32_t)0x00000B00)
#define RCC_PLL2Mul_14              ((uint32_t)0x00000C00)
#define RCC_PLL2Mul_16              ((uint32_t)0x00000E00)
#define RCC_PLL2Mul_20               ((uint32_t)0x00000F00)
//PLL3_multiplication_factor
#define RCC_PLL3Mul_8               ((uint32_t)0x00006000)
#define RCC_PLL3Mul_9               ((uint32_t)0x00007000)
#define RCC_PLL3Mul_10              ((uint32_t)0x00008000)
#define RCC_PLL3Mul_11               ((uint32_t)0x00009000)
#define RCC_PLL3Mul_12               ((uint32_t)0x0000A000)
#define RCC_PLL3Mul_13               ((uint32_t)0x0000B000)
#define RCC_PLL3Mul_14               ((uint32_t)0x0000C000)
#define RCC_PLL3Mul_16              ((uint32_t)0x0000E000)
#define RCC_PLL3Mul_20               ((uint32_t)0x0000F000)
//System_clock_source  
#define RCC_SYSCLKSource_HSI         ((uint32_t)0x00000000)    //Abfrage auf 0x00    //High Speed Indern Oszilator = 8MHz
#define RCC_SYSCLKSource_HSE         ((uint32_t)0x00000001)     //Abfrage auf 0x04    //High Speed Extern Oszilator = 25MHz
#define RCC_SYSCLKSource_PLLCLK     ((uint32_t)0x00000002)    //Abfrage auf 0x08    //PLL
//AHB_clock_source
#define RCC_SYSCLK_Div1             ((uint32_t)0x00000000)
#define RCC_SYSCLK_Div2              ((uint32_t)0x00000080)
#define RCC_SYSCLK_Div4             ((uint32_t)0x00000090)
#define RCC_SYSCLK_Div8             ((uint32_t)0x000000A0)
#define RCC_SYSCLK_Div16           ((uint32_t)0x000000B0)
#define RCC_SYSCLK_Div64             ((uint32_t)0x000000C0)
#define RCC_SYSCLK_Div128          ((uint32_t)0x000000D0)
#define RCC_SYSCLK_Div256            ((uint32_t)0x000000E0)
#define RCC_SYSCLK_Div512            ((uint32_t)0x000000F0)
//APB1_APB2_clock_source
#define RCC_HCLK_Div1               ((uint32_t)0x00000000)
#define RCC_HCLK_Div2                ((uint32_t)0x00000400)
#define RCC_HCLK_Div4                ((uint32_t)0x00000500)
#define RCC_HCLK_Div8                ((uint32_t)0x00000600)
#define RCC_HCLK_Div16               ((uint32_t)0x00000700)
//RCC_Interrupt_source             
#define RCC_IT_LSIRDY                ((uint8_t)0x01)
#define RCC_IT_LSERDY                ((uint8_t)0x02)
#define RCC_IT_HSIRDY                ((uint8_t)0x04)
#define RCC_IT_HSERDY                ((uint8_t)0x08)
#define RCC_IT_PLLRDY                ((uint8_t)0x10)
#define RCC_IT_CSS                   ((uint8_t)0x80)
//I2S2_clock_source
#define RCC_I2S2CLKSource_SYSCLK     ((uint8_t)0x00)
#define RCC_I2S2CLKSource_PLL3_VCO   ((uint8_t)0x01)
//I2S3_clock_source 
#define RCC_I2S3CLKSource_SYSCLK     ((uint8_t)0x00)
#define RCC_I2S3CLKSource_PLL3_VCO   ((uint8_t)0x01)   
//RTC_clock_source 
#define RCC_RTCCLKSource_LSE        ((uint32_t)0x00000100)
#define RCC_RTCCLKSource_LSI         ((uint32_t)0x00000200)
#define RCC_RTCCLKSource_HSE_Div128  ((uint32_t)0x00000300)  
//AHB_peripheral 
#define RCC_AHBPeriph_DMA1           ((uint32_t)0x00000001)
#define RCC_AHBPeriph_DMA2           ((uint32_t)0x00000002)
#define RCC_AHBPeriph_SRAM         ((uint32_t)0x00000004)
#define RCC_AHBPeriph_FLITF         ((uint32_t)0x00000010)
#define RCC_AHBPeriph_CRC          ((uint32_t)0x00000040)
#define RCC_AHBPeriph_FSMC          ((uint32_t)0x00000100)
#define RCC_AHBPeriph_SDIO           ((uint32_t)0x00000400) 
//APB2_peripheral 
#define RCC_APB2Periph_AFIO         ((uint32_t)0x00000001)
#define RCC_APB2Periph_GPIOA       ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOB       ((uint32_t)0x00000008)
#define RCC_APB2Periph_GPIOC       ((uint32_t)0x00000010)
#define RCC_APB2Periph_GPIOD       ((uint32_t)0x00000020)
#define RCC_APB2Periph_GPIOE       ((uint32_t)0x00000040)
#define RCC_APB2Periph_GPIOF        ((uint32_t)0x00000080)
#define RCC_APB2Periph_GPIOG       ((uint32_t)0x00000100)
#define RCC_APB2Periph_ADC1         ((uint32_t)0x00000200)
#define RCC_APB2Periph_ADC2         ((uint32_t)0x00000400)
#define RCC_APB2Periph_TIM1          ((uint32_t)0x00000800)
#define RCC_APB2Periph_SPI1        ((uint32_t)0x00001000)
#define RCC_APB2Periph_TIM8         ((uint32_t)0x00002000)
#define RCC_APB2Periph_USART1       ((uint32_t)0x00004000)
#define RCC_APB2Periph_ADC3         ((uint32_t)0x00008000)    
//APB1_peripheral 
#define RCC_APB1Periph_TIM2        ((uint32_t)0x00000001)
#define RCC_APB1Periph_TIM3         ((uint32_t)0x00000002)
#define RCC_APB1Periph_TIM4         ((uint32_t)0x00000004)
#define RCC_APB1Periph_TIM5         ((uint32_t)0x00000008)
#define RCC_APB1Periph_TIM6         ((uint32_t)0x00000010)
#define RCC_APB1Periph_TIM7          ((uint32_t)0x00000020)
#define RCC_APB1Periph_WWDG         ((uint32_t)0x00000800)
#define RCC_APB1Periph_SPI2         ((uint32_t)0x00004000)
#define RCC_APB1Periph_SPI3          ((uint32_t)0x00008000)
#define RCC_APB1Periph_USART2        ((uint32_t)0x00020000)
#define RCC_APB1Periph_USART3      ((uint32_t)0x00040000)
#define RCC_APB1Periph_UART4         ((uint32_t)0x00080000)
#define RCC_APB1Periph_UART5         ((uint32_t)0x00100000)
#define RCC_APB1Periph_I2C1        ((uint32_t)0x00200000)
#define RCC_APB1Periph_I2C2         ((uint32_t)0x00400000)
#define RCC_APB1Periph_USB        ((uint32_t)0x00800000)
#define RCC_APB1Periph_CAN1        ((uint32_t)0x02000000)
#define RCC_APB1Periph_BKP          ((uint32_t)0x08000000)
#define RCC_APB1Periph_PWR          ((uint32_t)0x10000000)
#define RCC_APB1Periph_DAC          ((uint32_t)0x20000000)
#define RCC_APB1Periph_CAN2        ((uint32_t)0x04000000) 
//RCC-Flags                    
#define RCC_FLAG_HSERDY            ((uint8_t)0x31)
#define RCC_FLAG_PLLRDY             ((uint8_t)0x39)
#define RCC_FLAG_PLL2RDY            ((uint8_t)0x3B) 
#define RCC_FLAG_PLL3RDY            ((uint8_t)0x3D)

#define RCC_OFFSET                  (RCC_BASE - PERIPH_BASE) 
#define CR_OFFSET                   (RCC_OFFSET + 0x00)
//Alias word address of PLL2ON bit
#define PLL2ON_BitNumber            0x1A 
#define CR_PLL2ON_BB                (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLL2ON_BitNumber * 4))
//Alias word address of PLL3ON bit 
#define PLL3ON_BitNumber            0x1C
#define CR_PLL3ON_BB                (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLL3ON_BitNumber * 4))
//CFGR2 register bit mask
#define CFGR2_PREDIV1SRC           ((uint32_t)0x00010000)
#define CFGR2_PREDIV1              ((uint32_t)0x0000000F)
#define CFGR2_PREDIV2              ((uint32_t)0x000000F0)
#define CFGR2_PLL2MUL              ((uint32_t)0x00000F00)
#define CFGR2_PLL3MUL              ((uint32_t)0x0000F000)
//Alias word address of I2S2SRC bit
#define CFGR2_OFFSET                (RCC_OFFSET + 0x2C)
#define I2S2SRC_BitNumber           0x11
#define CFGR2_I2S2SRC_BB            (PERIPH_BB_BASE + (CFGR2_OFFSET * 32) + (I2S2SRC_BitNumber * 4))
//Alias word address of I2S3SRC bit
#define I2S3SRC_BitNumber           0x12
#define CFGR2_I2S3SRC_BB            (PERIPH_BB_BASE + (CFGR2_OFFSET * 32) + (I2S3SRC_BitNumber * 4))
//Alias word address of PLLON bit    
#define PLLON_BitNumber             0x18
#define CR_PLLON_BB                 (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLON_BitNumber * 4))
//RTC_clock_source
#define RCC_RTCCLKSource_LSE         ((uint32_t)0x00000100)
#define RCC_RTCCLKSource_LSI         ((uint32_t)0x00000200)
#define RCC_RTCCLKSource_HSE_Div128  ((uint32_t)0x00000300)
//Set_SysClk
#define SysCLK_HSI          0x01
#define SysCLK_HSE          0x02
#define SysCLK_PLL          0x04
//Flash      
#define FLASH_Latency_2             ((uint32_t)0x00000002)  
#define ACR_PRFTBE_Mask            ((uint32_t)0xFFFFFFEF) 
#define ACR_LATENCY_Mask          ((uint32_t)0x00000038)
#define FLASH_PrefetchBuffer_Enable ((uint32_t)0x00000010)


#define HSE_Value            ((uint32_t)25000000) /*!< Value of the External oscillator in Hz */
  
/********************************************************************************************/
/*    Strukturen                                      */
/********************************************************************************************/        
typedef struct
{
  uint32_t SYSCLK_Frequency;  /*!< returns SYSCLK clock frequency expressed in Hz */
  uint32_t HCLK_Frequency;    /*!< returns HCLK clock frequency expressed in Hz */
  uint32_t PCLK1_Frequency;   /*!< returns PCLK1 clock frequency expressed in Hz */
  uint32_t PCLK2_Frequency;   /*!< returns PCLK2 clock frequency expressed in Hz */
  uint32_t ADCCLK_Frequency;  /*!< returns ADCCLK clock frequency expressed in Hz */
}RCC_ClocksTypeDef;

#ifdef __RCC_LIB_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/****************************************************************************/
/*                                                */
/****************************************************************************/ 
EXPORT void RCC_DeInit(void);
EXPORT void RCC_HSEConfig(void);
EXPORT int  RCC_WaitForHSEStartUp(void);
EXPORT int RCC_GetFlagStatus(uint8_t RCC_FLAG); 
EXPORT void RCC_HCLKConfig(void);
EXPORT void RCC_PCLK2Config(void);   
EXPORT void RCC_PCLK1Config(void);
EXPORT void RCC_PREDIV2Config(uint32_t RCC_PREDIV2_Div);
EXPORT void RCC_PLL2Config(uint32_t RCC_PLL2Mul);
EXPORT void RCC_PLL2Cmd(FunctionalState NewState);
EXPORT void RCC_PLL3Config(uint32_t RCC_PLL3Mul);
EXPORT void RCC_PLL3Cmd(FunctionalState NewState);
EXPORT void RCC_PREDIV1Config(uint32_t RCC_PREDIV1_Source, uint32_t RCC_PREDIV1_Div);
EXPORT void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLMul);
EXPORT void RCC_I2S2CLKConfig(uint32_t RCC_I2S2CLKSource);
EXPORT void RCC_I2S3CLKConfig(uint32_t RCC_I2S3CLKSource);
EXPORT void RCC_PLLCmd(FunctionalState NewState);
EXPORT void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource);
EXPORT uint8_t RCC_GetSYSCLKSource(void);
EXPORT void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);
EXPORT void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);
EXPORT void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
EXPORT void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
EXPORT void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks);
EXPORT void Set_SysCLK(char SelSysCLK);     
EXPORT void FLASH_PrefetchBufferCmd(void);
EXPORT void FLASH_SetLatency(void);

#undef  EXPORT
#endif

