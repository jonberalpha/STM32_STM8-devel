#ifndef  __ADC_DAC_LIB_H     //keine Mehrfach definition
#define  __ADC_DAC_LIB_H


/****************************************************************************/
/*                        Definitionen                                      */
/****************************************************************************/ 
#define addr_ADC            0x8E 
#define addr_DAC            0x34



#ifdef __ADC_DAC_LIB_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/****************************************************************************/
/*                                                                          */
/****************************************************************************/ 
EXPORT   void DAC_init(void);    
EXPORT   void ADC_init(void);
EXPORT  void write_reg(char addr, char reg_addr, char wert);

#undef  EXPORT
#endif  /* __ADC_DAC_CONFIG_H*/
