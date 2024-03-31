#ifndef  __DMA_LIB_H     //keine Mehrfach definition
#define  __DMA_LIB_H

/****************************************************************************/
/*                              Defines                                     */
/****************************************************************************/
/* --------------------- DMA data transfer direction -----------------------*/
#define DMA_DIR_PeripheralDST              ((u32)0x00000010)
#define DMA_DIR_PeripheralSRC              ((u32)0x00000000)
/* ----------------------DMA peripheral incremented mode -------------------*/
#define DMA_PeripheralInc_Enable           ((u32)0x00000040)
#define DMA_PeripheralInc_Disable          ((u32)0x00000000)
/* ------------------------- DMA memory incremented mode -------------------*/
#define DMA_MemoryInc_Enable               ((u32)0x00000080)
#define DMA_MemoryInc_Disable              ((u32)0x00000000)
/* ------------------------ DMA peripheral data size ------------------------*/
#define DMA_PeripheralDataSize_Byte        ((u32)0x00000000)
#define DMA_PeripheralDataSize_HalfWord    ((u32)0x00000100)
#define DMA_PeripheralDataSize_Word        ((u32)0x00000200)
/* -------------------------- DMA memory data size --------------------------*/
#define DMA_MemoryDataSize_Byte            ((u32)0x00000000)
#define DMA_MemoryDataSize_HalfWord        ((u32)0x00000400)
#define DMA_MemoryDataSize_Word            ((u32)0x00000800)
/* ----------------------------- DMA circular/normal mode -------------------*/
#define DMA_Mode_Circular                  ((u32)0x00000020)
#define DMA_Mode_Normal                    ((u32)0x00000000)
/* --------------------------- DMA priority level ---------------------------*/
#define DMA_Priority_VeryHigh              ((u32)0x00003000)
#define DMA_Priority_High                  ((u32)0x00002000)
#define DMA_Priority_Medium                ((u32)0x00001000)
#define DMA_Priority_Low                   ((u32)0x00000000)
/* --------------------------- DMA memory to memory -------------------------*/
#define DMA_M2M_Enable                     ((u32)0x00004000)
#define DMA_M2M_Disable                    ((u32)0x00000000)
/* -------------------------- DMA interrupts definition ---------------------*/
#define DMA_IT_TC                          ((u32)0x00000002)
#define DMA_IT_HT                          ((u32)0x00000004)
#define DMA_IT_TE                          ((u32)0x00000008)
/* ----------------------------- For DMA1 -----------------------------------*/
#define DMA1_IT_GL1                        ((u32)0x00000001)
#define DMA1_IT_TC1                        ((u32)0x00000002)
#define DMA1_IT_HT1                        ((u32)0x00000004)
#define DMA1_IT_TE1                        ((u32)0x00000008)
#define DMA1_IT_GL2                        ((u32)0x00000010)
#define DMA1_IT_TC2                        ((u32)0x00000020)
#define DMA1_IT_HT2                        ((u32)0x00000040)
#define DMA1_IT_TE2                        ((u32)0x00000080)
#define DMA1_IT_GL3                        ((u32)0x00000100)
#define DMA1_IT_TC3                        ((u32)0x00000200)
#define DMA1_IT_HT3                        ((u32)0x00000400)
#define DMA1_IT_TE3                        ((u32)0x00000800)
#define DMA1_IT_GL4                        ((u32)0x00001000)
#define DMA1_IT_TC4                        ((u32)0x00002000)
#define DMA1_IT_HT4                        ((u32)0x00004000)
#define DMA1_IT_TE4                        ((u32)0x00008000)
#define DMA1_IT_GL5                        ((u32)0x00010000)
#define DMA1_IT_TC5                        ((u32)0x00020000)
#define DMA1_IT_HT5                        ((u32)0x00040000)
#define DMA1_IT_TE5                        ((u32)0x00080000)
#define DMA1_IT_GL6                        ((u32)0x00100000)
#define DMA1_IT_TC6                        ((u32)0x00200000)
#define DMA1_IT_HT6                        ((u32)0x00400000)
#define DMA1_IT_TE6                        ((u32)0x00800000)
#define DMA1_IT_GL7                        ((u32)0x01000000)
#define DMA1_IT_TC7                        ((u32)0x02000000)
#define DMA1_IT_HT7                        ((u32)0x04000000)
#define DMA1_IT_TE7                        ((u32)0x08000000)
/* - For DMA2 ---*/
#define DMA2_IT_GL1                        ((u32)0x10000001)
#define DMA2_IT_TC1                        ((u32)0x10000002)
#define DMA2_IT_HT1                        ((u32)0x10000004)
#define DMA2_IT_TE1                        ((u32)0x10000008)
#define DMA2_IT_GL2                        ((u32)0x10000010)
#define DMA2_IT_TC2                        ((u32)0x10000020)
#define DMA2_IT_HT2                        ((u32)0x10000040)
#define DMA2_IT_TE2                        ((u32)0x10000080)
#define DMA2_IT_GL3                        ((u32)0x10000100)
#define DMA2_IT_TC3                        ((u32)0x10000200)
#define DMA2_IT_HT3                        ((u32)0x10000400)
#define DMA2_IT_TE3                        ((u32)0x10000800)
#define DMA2_IT_GL4                        ((u32)0x10001000)
#define DMA2_IT_TC4                        ((u32)0x10002000)
#define DMA2_IT_HT4                        ((u32)0x10004000)
#define DMA2_IT_TE4                        ((u32)0x10008000)
#define DMA2_IT_GL5                        ((u32)0x10010000)
#define DMA2_IT_TC5                        ((u32)0x10020000)
#define DMA2_IT_HT5                        ((u32)0x10040000)
#define DMA2_IT_TE5                        ((u32)0x10080000)
/* -------------------------------- DMA flags definition --------------------*/
/* For DMA1 */
#define DMA1_FLAG_GL1                      ((u32)0x00000001)
#define DMA1_FLAG_TC1                      ((u32)0x00000002)
#define DMA1_FLAG_HT1                      ((u32)0x00000004)
#define DMA1_FLAG_TE1                      ((u32)0x00000008)
#define DMA1_FLAG_GL2                      ((u32)0x00000010)
#define DMA1_FLAG_TC2                      ((u32)0x00000020)
#define DMA1_FLAG_HT2                      ((u32)0x00000040)
#define DMA1_FLAG_TE2                      ((u32)0x00000080)
#define DMA1_FLAG_GL3                      ((u32)0x00000100)
#define DMA1_FLAG_TC3                      ((u32)0x00000200)
#define DMA1_FLAG_HT3                      ((u32)0x00000400)
#define DMA1_FLAG_TE3                      ((u32)0x00000800)
#define DMA1_FLAG_GL4                      ((u32)0x00001000)
#define DMA1_FLAG_TC4                      ((u32)0x00002000)
#define DMA1_FLAG_HT4                      ((u32)0x00004000)
#define DMA1_FLAG_TE4                      ((u32)0x00008000)
#define DMA1_FLAG_GL5                      ((u32)0x00010000)
#define DMA1_FLAG_TC5                      ((u32)0x00020000)
#define DMA1_FLAG_HT5                      ((u32)0x00040000)
#define DMA1_FLAG_TE5                      ((u32)0x00080000)
#define DMA1_FLAG_GL6                      ((u32)0x00100000)
#define DMA1_FLAG_TC6                      ((u32)0x00200000)
#define DMA1_FLAG_HT6                      ((u32)0x00400000)
#define DMA1_FLAG_TE6                      ((u32)0x00800000)
#define DMA1_FLAG_GL7                      ((u32)0x01000000)
#define DMA1_FLAG_TC7                      ((u32)0x02000000)
#define DMA1_FLAG_HT7                      ((u32)0x04000000)
#define DMA1_FLAG_TE7                      ((u32)0x08000000)
/* For DMA2 */
#define DMA2_FLAG_GL1                      ((u32)0x10000001)
#define DMA2_FLAG_TC1                      ((u32)0x10000002)
#define DMA2_FLAG_HT1                      ((u32)0x10000004)
#define DMA2_FLAG_TE1                      ((u32)0x10000008)
#define DMA2_FLAG_GL2                      ((u32)0x10000010)
#define DMA2_FLAG_TC2                      ((u32)0x10000020)
#define DMA2_FLAG_HT2                      ((u32)0x10000040)
#define DMA2_FLAG_TE2                      ((u32)0x10000080)
#define DMA2_FLAG_GL3                      ((u32)0x10000100)
#define DMA2_FLAG_TC3                      ((u32)0x10000200)
#define DMA2_FLAG_HT3                      ((u32)0x10000400)
#define DMA2_FLAG_TE3                      ((u32)0x10000800)
#define DMA2_FLAG_GL4                      ((u32)0x10001000)
#define DMA2_FLAG_TC4                      ((u32)0x10002000)
#define DMA2_FLAG_HT4                      ((u32)0x10004000)
#define DMA2_FLAG_TE4                      ((u32)0x10008000)
#define DMA2_FLAG_GL5                      ((u32)0x10010000)
#define DMA2_FLAG_TC5                      ((u32)0x10020000)
#define DMA2_FLAG_HT5                      ((u32)0x10040000)
#define DMA2_FLAG_TE5                      ((u32)0x10080000)
/* --------------------------- DMA Buffer Size ------------------------------*/
#define IS_DMA_BUFFER_SIZE(SIZE) (((SIZE) >= 0x1) && ((SIZE) < 0x10000))
/* ------------------------------ DMA ENABLE mask ---------------------------*/
#define CCR_ENABLE_Set          ((uint32_t)0x00000001)
#define CCR_ENABLE_Reset        ((uint32_t)0xFFFFFFFE)
/* --------------------------DMA1 Channelx interrupt pending bit masks ------*/
#define DMA1_Channel1_IT_Mask    ((uint32_t)0x0000000F)
#define DMA1_Channel2_IT_Mask    ((uint32_t)0x000000F0)
#define DMA1_Channel3_IT_Mask    ((uint32_t)0x00000F00)
#define DMA1_Channel4_IT_Mask    ((uint32_t)0x0000F000)
#define DMA1_Channel5_IT_Mask    ((uint32_t)0x000F0000)
#define DMA1_Channel6_IT_Mask    ((uint32_t)0x00F00000)
#define DMA1_Channel7_IT_Mask    ((uint32_t)0x0F000000)
/* -------------------- DMA2 Channelx interrupt pending bit masks -----------*/
#define DMA2_Channel1_IT_Mask    ((uint32_t)0x0000000F)
#define DMA2_Channel2_IT_Mask    ((uint32_t)0x000000F0)
#define DMA2_Channel3_IT_Mask    ((uint32_t)0x00000F00)
#define DMA2_Channel4_IT_Mask    ((uint32_t)0x0000F000)
#define DMA2_Channel5_IT_Mask    ((uint32_t)0x000F0000)
/* -------------------------- DMA2 FLAG mask --------------------------------*/
#define FLAG_Mask_DMA                ((uint32_t)0x10000000)
/* --------------------------- DMA registers Masks --------------------------*/
#define CCR_CLEAR_Mask           ((uint32_t)0xFFFF800F)  


#define SPI2_DR_Address    ((u32)0x4000380C)
#define SPI3_DR_Address    ((u32)0x40003C0C)
/*****************************************************************************/
/*    Strukturen                                                   */
/*****************************************************************************/
typedef struct
{
  u32 DMA_PeripheralBaseAddr;
  u32 DMA_MemoryBaseAddr;
  u32 DMA_DIR;
  u32 DMA_BufferSize;
  u32 DMA_PeripheralInc;
  u32 DMA_MemoryInc;
  u32 DMA_PeripheralDataSize;
  u32 DMA_MemoryDataSize;
  u32 DMA_Mode;
  u32 DMA_Priority;
  u32 DMA_M2M;
}DMA_InitTypeDef;

#ifdef __DMA_LIB_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/****************************************************************************/
/*                                                */
/****************************************************************************/ 
EXPORT void DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct);
EXPORT void DMA_Cmd(DMA_Channel_TypeDef* DMAy_Channelx, FunctionalState NewState);     
EXPORT void DMA_ITConfig(DMA_Channel_TypeDef* DMAy_Channelx, uint32_t DMA_IT, FunctionalState NewState); 
EXPORT void DMA_ClearITPendingBit(uint32_t DMA_IT);  
EXPORT void DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx);

#undef  EXPORT
#endif

