#define __ADC_DAC_LIB_MOD
#include "i2c_lib.h"
#include "gpio_lib.h"
#include "adc_dac_lib.h"

/****************************************************************************/
/*    Konstanten                                                            */
/****************************************************************************/ 
USART_TypeDef* uart=USART3;  
USART_TypeDef* uart_2=USART2;

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void DAC_init(void)
{
  I2C_error_message(uart_2, I2Cx_init(I2C1));    //I2C1-init, sonst Fehlermeldung über uart2
                              
  write_reg(addr_DAC, 0x00, 0x80);        //Addr:0000000 Daten:010000000   Left Line Input Channel Control (Defaultmäßig eingestellt)
  write_reg(addr_DAC, 0x02, 0x80);        //Addr:0000001 Daten:010000000   Right Line Input Channel Control (Defaultmäßig eingestellt)
  write_reg(addr_DAC, 0x05, 0xF9);        //Addr:0000010 Daten:111111001   Left Channel Headphone Volume Control (default= 0dB = 1111001
  write_reg(addr_DAC, 0x07, 0xF9);        //Addr:0000011 Daten:111111001   Right Channel Headphone Volume Control  (default= 0dB = 1111001   )
  write_reg(addr_DAC, 0x08, 0x12);        //Addr:0000100 Daten:000010010   Analog Audio Path Control: DAC enabled, Bypass Disabled
  write_reg(addr_DAC, 0x0A, 0x00);        //Addr:0000101 Daten:000000000   Digital Audio Path Control: DAC soft mute disabled
  write_reg(addr_DAC, 0x0C, 0x07);        //Addr:0000110 Daten:000100111   Power Down Control: DAC = ON (0) , OUT = ON (0)
  write_reg(addr_DAC, 0x0E, 0x02);        //Addr:0000111 Daten:000000010   Digital Audio Interface Format: I2S-Format, MSBfirst, left - 1 aligned
  write_reg(addr_DAC, 0x10, 0x00);        //Addr:0001000 Daten:000000000   Sample Rate Control: CLKIN = 0 ==> MCLK , fs = 48kHz => 0000 , FilterType 1  
  write_reg(addr_DAC, 0x12, 0x01);        //Addr:0001001 Daten:000000001   Digital Interface Activation : Activate = 1 (Activate)

  GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);  //SPI3 remappen , da die Ports schon von anderen besetzt sind (z.B. Uart4)
  I2Cx_disable(I2C1);                //I2C1 disablen
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void ADC_init(void)
{       
  I2C_error_message(uart_2, I2Cx_init(I2C1));     //I2C1-init, sonst Fehlermeldung über uart2
  
  /*Abfolge der Initialisierung*/
  //Audio-Interface
  //Gain PG1,PG2
  //Analog-Bias-Powerup
  //Zero-Cross detection enable
  //Vcom power up
  //Analog front end (ADL, ADR, D2S, MCB, PG1, 2, 5, 6) power up
  //Analog input (MUX3, MUX4) select. Analog input (MUX1, MUX2) select
  //Analog input L-ch (PG3) volume (0 dB)
  //Analog input R-ch (PG4) volume (0 dB)
  write_reg(addr_ADC, 0x51, 0x00);  //Mute=disable, Interface=I2S-Standard
  write_reg(addr_ADC, 0x5A, 0x00);  //PG2,1 = 0dB
  write_reg(addr_ADC, 0x7C, 0x00);  //PG3,4 = 0dB
  write_reg(addr_ADC, 0x49, 0x80);  //Analog Bias power up
  write_reg(addr_ADC, 0x55, 0x00);  //fs=48000Hz                
  write_reg(addr_ADC, 0x56, 0x21);  //Zero-Cross detection enable, Sysclk=12.288MHz
  write_reg(addr_ADC, 0x4A, 0x01);  //Vcom power up
  write_reg(addr_ADC, 0x52, 0x77);  //Analog front end (ADL, ADR, D2S, MCB, PG1, 2, 5, 6) power up    evtl. 0x77
  write_reg(addr_ADC, 0x57, 0x11);  //AIN1L AIN1R selected = Klinke
  write_reg(addr_ADC, 0x4F, 0x0C);  //Analog input L-ch (PG3) volume (0 dB)
  write_reg(addr_ADC, 0x50, 0x0C);  //Analog input R-ch (PG4) volume (0 dB)   
  
  /*  geordnet
  write_reg(addr_ADC, 0x49, 0x80);  //Analog Bias power up
  write_reg(addr_ADC, 0x4A, 0x01);  //Vcom power up
  write_reg(addr_ADC, 0x4F, 0x0C);  //Analog input L-ch (PG3) volume (0 dB)
  write_reg(addr_ADC, 0x50, 0x0C);  //Analog input R-ch (PG4) volume (0 dB)   
  write_reg(addr_ADC, 0x51, 0x00);  //Mute=disable, Interface=I2S-Standard
  write_reg(addr_ADC, 0x52, 0x77);  //Analog front end (ADL, ADR, D2S, MCB, PG1, 2, 5, 6) power up    evtl. 0x77
  write_reg(addr_ADC, 0x55, 0x00);  //fs=48000Hz                
  write_reg(addr_ADC, 0x56, 0x21);  //Zero-Cross detection enable, Sysclk=12.288MHz
  write_reg(addr_ADC, 0x57, 0x11);  //AIN1L AIN1R selected = Klinke
  write_reg(addr_ADC, 0x5A, 0x00);  //PG2,1 = 0dB
  write_reg(addr_ADC, 0x7C, 0x00);  //PG3,4 = 0dB
    
  */
  
  
  I2Cx_disable(I2C1);          //I2C1 disablen
}

/****************************************************************************************************************/
/*  write_reg:  Übergebenen Wert in übergebenes Register schreiben + Fehlerausgabe über UART                    */
/****************************************************************************************************************/
void write_reg(char addr, char reg_addr, char wert)
{  
  if(I2C_error_message(uart_2, I2Cx_start(I2C1,addr&0xFE,0))!=info_success )  //Startbedingung + Adresse für schreiben
  {
    for(;;);
  }   
  if(I2C_error_message(uart_2, I2Cx_send(I2C1,reg_addr,0))!=info_success )  //Zielspeicheradresse
  {
    for(;;);
  }        
  if(I2C_error_message(uart_2, I2Cx_send(I2C1,wert,1))!=info_success )    //zu schreibende Daten + Stopbedingung
  {
    for(;;);
  } 
}   
