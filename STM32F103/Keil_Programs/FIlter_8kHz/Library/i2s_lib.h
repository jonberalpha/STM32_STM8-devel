#ifndef  __I2S_LIB_H     //keine Mehrfach definition
#define  __I2S_LIB_H


/****************************************************************************/ 
/*                               Strukturen                  */
/****************************************************************************/
typedef struct
{

  uint16_t I2S_Mode;         /*!< Specifies the I2S operating mode.
                                  This parameter can be any combination of @ref I2S_Mode */

  uint16_t I2S_Standard;     /*!< Specifies the standard used for the I2S communication.
                                  This parameter can be any combination of @ref I2S_Standard */

  uint16_t I2S_DataFormat;   /*!< Specifies the data format for the I2S communication.
                                  This parameter can be any combination of @ref I2S_Data_Format */

  uint16_t I2S_MCLKOutput;   /*!< Specifies whether the I2S MCLK output is enabled or not.
                                  This parameter can be any combination of @ref I2S_MCLK_Output */

  uint32_t I2S_AudioFreq;    /*!< Specifies the frequency selected for the I2S communication.
                                  This parameter can be any combination of @ref I2S_Audio_Frequency */

  uint16_t I2S_CPOL;         /*!< Specifies the idle state of the I2S clock.
                                  This parameter can be any combination of @ref I2S_Clock_Polarity */
}I2S_InitTypeDef;


/*------------------------ I2S_Mode -----------------*/
#define I2S_Mode_SlaveTx            ((uint16_t)0x0000)
#define I2S_Mode_SlaveRx          ((uint16_t)0x0100)
#define I2S_Mode_MasterTx           ((uint16_t)0x0200)
#define I2S_Mode_MasterRx           ((uint16_t)0x0300)
/*------------------- I2S_Standard -------------------*/
#define I2S_Standard_Phillips     ((uint16_t)0x0000)
#define I2S_Standard_MSB             ((uint16_t)0x0010)
#define I2S_Standard_LSB            ((uint16_t)0x0020)
#define I2S_Standard_PCMShort        ((uint16_t)0x0030)
#define I2S_Standard_PCMLong         ((uint16_t)0x00B0)
/* ------------------ I2S_Data_Format -----------------*/
#define I2S_DataFormat_16b           ((uint16_t)0x0000)
#define I2S_DataFormat_16bextended   ((uint16_t)0x0001)
#define I2S_DataFormat_24b           ((uint16_t)0x0003)
#define I2S_DataFormat_32b           ((uint16_t)0x0005)
/* ------------------ I2S_MCLKOutput-------------------*/
#define I2S_MCLKOutput_Enable        ((uint16_t)0x0200)
#define I2S_MCLKOutput_Disable       ((uint16_t)0x0000)
/* ---------------------- I2S_AudioFreq -----------------*/
#define I2S_AudioFreq_96k            ((uint32_t)96000)
#define I2S_AudioFreq_48k           ((uint32_t)48000)
#define I2S_AudioFreq_44k            ((uint32_t)44100)
#define I2S_AudioFreq_32k            ((uint32_t)32000)
#define I2S_AudioFreq_22k            ((uint32_t)22050)
#define I2S_AudioFreq_16k           ((uint32_t)16000)
#define I2S_AudioFreq_11k           ((uint32_t)11025)
#define I2S_AudioFreq_8k            ((uint32_t)8000)
#define I2S_AudioFreq_Default       ((uint32_t)2)
/* ------------------- I2S_ClockPol ---------------------*/
#define I2S_CPOL_Low                 ((uint16_t)0x0000)
#define I2S_CPOL_High                ((uint16_t)0x0008)   
/* ------------------- I2S_init    ------------------------*/
#define I2S_Mode_Select          ((uint16_t)0x0800)
#define I2S2_CLOCK_SRC           ((uint32_t)(0x00020000))
#define I2S3_CLOCK_SRC           ((uint32_t)(0x00040000))
#define I2S_MUL_MASK             ((uint32_t)(0x0000F000))
#define I2S_DIV_MASK             ((uint32_t)(0x000000F0)) 
#define I2SCFGR_CLEAR_Mask       ((uint16_t)0xF040)
#define I2SCFGR_I2SE_Set         ((uint16_t)0x0400)
#define I2SCFGR_I2SE_Reset       ((uint16_t)0xFBFF)
#define SPI_I2S_IT_TXE            ((uint8_t)0x71)
#define SPI_I2S_FLAG_TXE            ((uint16_t)0x0002)
#define SPI_I2S_FLAG_CHSIDE         ((uint16_t)0x0004)
#define SPI_I2S_FLAG_RXNE            ((uint16_t)0x0001)

#ifdef __I2S_LIB_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/****************************************************************************/
/*                                                */
/****************************************************************************/ 
EXPORT   void DAC_init(void);    
EXPORT   void ADC_init(void);
EXPORT  void I2S_Config(void);  
EXPORT  void I2S_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState);   
EXPORT  void SPI3_I2S_DeInit(void);
EXPORT  void SPI2_I2S_DeInit(void);
EXPORT  void I2S_Init(SPI_TypeDef* SPIx, I2S_InitTypeDef* I2S_InitStruct);
EXPORT  void write_reg(char addr, char reg_addr, char wert);

#undef  EXPORT
#endif  /* __AUDIO_CONFIG_H*/
