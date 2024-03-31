#ifndef  __GPIO_LIB_H     //keine Mehrfach definition
#define  __GPIO_LIB_H

#include "armv20_std.h"

//Remap
#define GPIO_Remap_SPI1             ((uint32_t)0x00000001)  /*!< SPI1 Alternate Function mapping */
#define GPIO_Remap_I2C1             ((uint32_t)0x00000002)  /*!< I2C1 Alternate Function mapping */
#define GPIO_Remap_USART1           ((uint32_t)0x00000004)  /*!< USART1 Alternate Function mapping */
#define GPIO_Remap_USART2           ((uint32_t)0x00000008)  /*!< USART2 Alternate Function mapping */
#define GPIO_PartialRemap_USART3    ((uint32_t)0x00140010)  /*!< USART3 Partial Alternate Function mapping */
#define GPIO_FullRemap_USART3       ((uint32_t)0x00140030)  /*!< USART3 Full Alternate Function mapping */
#define GPIO_PartialRemap_TIM1      ((uint32_t)0x00160040)  /*!< TIM1 Partial Alternate Function mapping */
#define GPIO_FullRemap_TIM1         ((uint32_t)0x001600C0)  /*!< TIM1 Full Alternate Function mapping */
#define GPIO_PartialRemap1_TIM2     ((uint32_t)0x00180100)  /*!< TIM2 Partial1 Alternate Function mapping */
#define GPIO_PartialRemap2_TIM2     ((uint32_t)0x00180200)  /*!< TIM2 Partial2 Alternate Function mapping */
#define GPIO_FullRemap_TIM2         ((uint32_t)0x00180300)  /*!< TIM2 Full Alternate Function mapping */
#define GPIO_PartialRemap_TIM3      ((uint32_t)0x001A0800)  /*!< TIM3 Partial Alternate Function mapping */
#define GPIO_FullRemap_TIM3         ((uint32_t)0x001A0C00)  /*!< TIM3 Full Alternate Function mapping */
#define GPIO_Remap_TIM4             ((uint32_t)0x00001000)  /*!< TIM4 Alternate Function mapping */
#define GPIO_Remap1_CAN1            ((uint32_t)0x001D4000)  /*!< CAN1 Alternate Function mapping */
#define GPIO_Remap2_CAN1            ((uint32_t)0x001D6000)  /*!< CAN1 Alternate Function mapping */
#define GPIO_Remap_PD01             ((uint32_t)0x00008000)  /*!< PD01 Alternate Function mapping */
#define GPIO_Remap_TIM5CH4_LSI      ((uint32_t)0x00200001)  /*!< LSI connected to TIM5 Channel4 input capture for calibration */
#define GPIO_Remap_ADC1_ETRGINJ     ((uint32_t)0x00200002)  /*!< ADC1 External Trigger Injected Conversion remapping */
#define GPIO_Remap_ADC1_ETRGREG     ((uint32_t)0x00200004)  /*!< ADC1 External Trigger Regular Conversion remapping */
#define GPIO_Remap_ADC2_ETRGINJ     ((uint32_t)0x00200008)  /*!< ADC2 External Trigger Injected Conversion remapping */
#define GPIO_Remap_ADC2_ETRGREG     ((uint32_t)0x00200010)  /*!< ADC2 External Trigger Regular Conversion remapping */
#define GPIO_Remap_ETH              ((uint32_t)0x00200020)  /*!< Ethernet remapping (only for Connectivity line devices) */
#define GPIO_Remap_CAN2             ((uint32_t)0x00200040)  /*!< CAN2 remapping (only for Connectivity line devices) */
#define GPIO_Remap_SWJ_NoJTRST      ((uint32_t)0x00300100)  /*!< Full SWJ Enabled (JTAG-DP + SW-DP) but without JTRST */
#define GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x00300200)  /*!< JTAG-DP Disabled and SW-DP Enabled */
#define GPIO_Remap_SWJ_Disable      ((uint32_t)0x00300400)  /*!< Full SWJ Disabled (JTAG-DP + SW-DP) */
#define GPIO_Remap_SPI3             ((uint32_t)0x00201000)  /*!< SPI3 Alternate Function mapping (only for Connectivity line devices) */
#define GPIO_Remap_TIM2ITR1_PTP_SOF ((uint32_t)0x00202000)  /*!< Ethernet PTP output or USB OTG SOF (Start of Frame) connected
                                                                 to TIM2 Internal Trigger 1 for calibration
                                                                 (only for Connectivity line devices) */
#define GPIO_Remap_PTP_PPS          ((uint32_t)0x00204000)  /*!< Ethernet MAC PPS_PTS output on PB05 (only for Connectivity line devices) */

#define EVCR_PORTPINCONFIG_MASK     ((uint16_t)0xFF80)
#define LSB_MASK                    ((uint16_t)0xFFFF)
#define DBGAFR_POSITION_MASK        ((uint32_t)0x000F0000)
#define DBGAFR_SWJCFG_MASK          ((uint32_t)0xF0FFFFFF)
#define DBGAFR_LOCATION_MASK        ((uint32_t)0x00200000)
#define DBGAFR_NUMBITS_MASK         ((uint32_t)0x00100000)


//GPIO-  Konfigurationsmodus
#define GPO_pushpull   0x10
#define GPO_opendrain  0x14
#define AFO_pushpull  0x18
#define AFO_opendrain  0x1C
#define INPUT_analog  0x00
#define INPUT_floating  0x04
#define INPUT_pulldown  0x28
#define INPUT_pullup  0x48

//GPIO- max. Output Frequenz
#define frequency_2MHz  0x02
#define frequency_10MHz  0x01
#define frequency_50MHz 0x03

/*--------------------------- Port Pin Definitionen --------------------------*/
#define IN_PA0  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,0))  // PA0
#define IN_PA1  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,1))  // PA1
#define IN_PA2  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,2))  // PA2
#define IN_PA3  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,3))  // PA3
#define IN_PA4  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,4))  // PA4
#define IN_PA5  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,5))  // PA5
#define IN_PA6  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,6))  // PA6
#define IN_PA7  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,7))  // PA7
#define IN_PA8  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,8))  // PA8
#define IN_PA9  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,9))  // PA9
#define IN_PA10 *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,10))  // PA10
#define IN_PA11 *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,11))  // PA11
#define IN_PA12 *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,12))  // PA12
#define IN_PA13 *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,13))  // PA13
#define IN_PA14 *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,14))  // PA14
#define IN_PA15 *(volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,15))  // PA15

#define IN_PB0  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,0))  // PB0
#define IN_PB1  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,1))  // PB1
#define IN_PB2  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,2))  // PB2
#define IN_PB3  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,3))  // PB3
#define IN_PB4  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,4))  // PB4
#define IN_PB5  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,5))  // PB5
#define IN_PB6  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,6))  // PB6
#define IN_PB7  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,7))  // PB7
#define IN_PB8  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,8))  // PB8
#define IN_PB9  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,9))  // PB9
#define IN_PB10 *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,10))  // PB10
#define IN_PB11 *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,11))  // PB11
#define IN_PB12 *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,12))  // PB12
#define IN_PB13 *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,13))  // PB13
#define IN_PB14 *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,14))  // PB14
#define IN_PB15 *(volatile unsigned long *)(BITBAND_PERI(GPIOB_IDR,15))  // PB15

#define IN_PC0  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,0))  // PC0
#define IN_PC1  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,1))  // PC1
#define IN_PC2  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,2))  // PC2
#define IN_PC3  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,3))  // PC3
#define IN_PC4  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,4))  // PC4
#define IN_PC5  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,5))  // PC5
#define IN_PC6  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,6))  // PC6
#define IN_PC7  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,7))  // PC7
#define IN_PC8  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,8))  // PC8
#define IN_PC9  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,9))  // PC9
#define IN_PC10 *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,10))  // PC10
#define IN_PC11 *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,11))  // PC11
#define IN_PC12 *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,12))  // PC12
#define IN_PC13 *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,13))  // PC13
#define IN_PC14 *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,14))  // PC14
#define IN_PC15 *(volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,15))  // PC15

#define IN_PD0  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,0))  // PD0
#define IN_PD1  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,1))  // PD1
#define IN_PD2  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,2))  // PD2
#define IN_PD3  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,3))  // PD3
#define IN_PD4  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,4))  // PD4
#define IN_PD5  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,5))  // PD5
#define IN_PD6  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,6))  // PD6
#define IN_PD7  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,7))  // PD7
#define IN_PD8  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,8))  // PD8
#define IN_PD9  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,9))  // PD9
#define IN_PD10 *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,10))  // PD10
#define IN_PD11 *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,11))  // PD11
#define IN_PD12 *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,12))  // PD12
#define IN_PD13 *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,13))  // PD13
#define IN_PD14 *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,14))  // PD14
#define IN_PD15 *(volatile unsigned long *)(BITBAND_PERI(GPIOD_IDR,15))  // PD15

#define IN_PE0  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,0))  // PE0
#define IN_PE1  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,1))  // PE1
#define IN_PE2  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,2))  // PE2
#define IN_PE3  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,3))  // PE3
#define IN_PE4  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,4))  // PE4
#define IN_PE5  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,5))  // PE5
#define IN_PE6  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,6))  // PE6
#define IN_PE7  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,7))  // PE7
#define IN_PE8  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,8))  // PE8
#define IN_PE9  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,9))  // PE9
#define IN_PE10 *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,10))  // PE10
#define IN_PE11 *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,11))  // PE11
#define IN_PE12 *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,12))  // PE12
#define IN_PE13 *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,13))  // PE13
#define IN_PE14 *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,14))  // PE14
#define IN_PE15 *(volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,15))  // PE15


#define OUT_PA0  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,0))  // PA0
#define OUT_PA1  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,1))  // PA1
#define OUT_PA2  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,2))  // PA2
#define OUT_PA3  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,3))  // PA3
#define OUT_PA4  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,4))  // PA4
#define OUT_PA5  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,5))  // PA5
#define OUT_PA6  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,6))  // PA6
#define OUT_PA7  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,7))  // PA7
#define OUT_PA8  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,8))  // PA8
#define OUT_PA9  *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,9))  // PA9
#define OUT_PA10 *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,10))// PA10
#define OUT_PA11 *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,11))// PA11
#define OUT_PA12 *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,12))// PA12
#define OUT_PA13 *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,13))// PA13
#define OUT_PA14 *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,14))// PA14
#define OUT_PA15 *(volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,15))// PA15

#define OUT_PB0  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,0))  // PB0
#define OUT_PB1  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,1))  // PB1
#define OUT_PB2  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,2))  // PB2
#define OUT_PB3  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,3))  // PB3
#define OUT_PB4  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,4))  // PB4
#define OUT_PB5  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,5))  // PB5
#define OUT_PB6  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,6))  // PB6
#define OUT_PB7  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,7))  // PB7
#define OUT_PB8  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,8))  // PB8
#define OUT_PB9  *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,9))  // PB9
#define OUT_PB10 *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,10))// PB10
#define OUT_PB11 *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,11))// PB11
#define OUT_PB12 *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,12))// PB12
#define OUT_PB13 *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,13))// PB13
#define OUT_PB14 *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,14))// PB14
#define OUT_PB15 *(volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,15))// PB15

#define OUT_PC0  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,0))  // PC0
#define OUT_PC1  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,1))  // PC1
#define OUT_PC2  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,2))  // PC2
#define OUT_PC3  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,3))  // PC3
#define OUT_PC4  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,4))  // PC4
#define OUT_PC5  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,5))  // PC5
#define OUT_PC6  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,6))  // PC6
#define OUT_PC7  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,7))  // PC7
#define OUT_PC8  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,8))  // PC8
#define OUT_PC9  *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,9))  // PC9
#define OUT_PC10 *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,10))// PC10
#define OUT_PC11 *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,11))// PC11
#define OUT_PC12 *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,12))// PC12
#define OUT_PC13 *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,13))// PC13
#define OUT_PC14 *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,14))// PC14
#define OUT_PC15 *(volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,15))// PC15

#define OUT_PD0  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,0))  // PD0
#define OUT_PD1  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,1))  // PD1
#define OUT_PD2  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,2))  // PD2
#define OUT_PD3  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,3))  // PD3
#define OUT_PD4  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,4))  // PD4
#define OUT_PD5  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,5))  // PD5
#define OUT_PD6  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,6))  // PD6
#define OUT_PD7  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,7))  // PD7
#define OUT_PD8  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,8))  // PD8
#define OUT_PD9  *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,9))  // PD9
#define OUT_PD10 *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,10))// PD10
#define OUT_PD11 *(volatile unsigned long *)(BITBAND_PERI(GIOD_ODR,11))  // PD11
#define OUT_PD12 *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,12))// PD12
#define OUT_PD13 *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,13))// PD13
#define OUT_PD14 *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,14))// PD14
#define OUT_PD15 *(volatile unsigned long *)(BITBAND_PERI(GPIOD_ODR,15))// PD15

#define OUT_PE0  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,0))  // PE0
#define OUT_PE1  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,1))  // PE1
#define OUT_PE2  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,2))  // PE2
#define OUT_PE3  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,3))  // PE3
#define OUT_PE4  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,4))  // PE4
#define OUT_PE5  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,5))  // PE5
#define OUT_PE6  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,6))  // PE6
#define OUT_PE7  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,7))  // PE7
#define OUT_PE8  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,8))  // PE8
#define OUT_PE9  *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,9))  // PE9
#define OUT_PE10 *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,10))// PE10
#define OUT_PE11 *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,11))// PE11
#define OUT_PE12 *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,12))// PE12
#define OUT_PE13 *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,13))// PE13
#define OUT_PE14 *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,14))// PE14
#define OUT_PE15 *(volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,15))// PE15

#ifdef __GPIO_LIB_H_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/****************************************************************************/
/*                                                */
/****************************************************************************/ 
EXPORT void init_port(GPIO_TypeDef* GPIO,char from,char to,char mode,char frequency);
EXPORT void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState);

#undef  EXPORT

#endif
