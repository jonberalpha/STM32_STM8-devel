 
/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team+Müllner+RM
* Version            : V2.2.0
* Date               : 06/13/2008 bis 12.7.2012
* Description        : Virtual Com Port Demo main file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x_lib.h"
#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "vcom.h"
#include "LCD.h"   //vom Keil Eval Board  Beispiel entnommen LCD_4bit.c auch

 
extern u32 count_out;
extern u32 count_in;
extern u8 buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];
int rx_flag=0, tx_flag=0;

unsigned short int  ADC_ConvertedValue;
int AD_value, i;
unsigned       char Clock1s;
unsigned       char c;	 
char str[20]  ;
								    
/*******************************************************************************
* Function Name  : main.
*******************************************************************************/
/*------------------------------------------------------------------------------
  Initialises the Analog/Digital converter
  PA1 (ADC Channel1) is used as analog input
  use DMA Channel1 for ADC1 (see DMA request mapping)
 *------------------------------------------------------------------------------*/
void adc_Init (void) {

 // GPIOC->CRL &= ~0x000F0000;                    // set PIN14  rm as analog input (see stm32_Init.c)
 // GPIOB->CRL &= ~0x000F0000;
  RCC->AHBENR |= (1<<0);                          // enable periperal clock for DMA

  DMA1_Channel1->CMAR  = (u32)&ADC_ConvertedValue;// set channel1 memory address
  DMA1_Channel1->CPAR  = (u32)&(ADC1->DR);        // set channel1 peripheral address
  DMA1_Channel1->CNDTR = 1;                       // transmit 1 word
  DMA1_Channel1->CCR   = 0x00002520;              // configure DMA channel
  DMA1_Channel1->CCR  |= (1 << 0);                // DMA Channel 1 enable
  RCC->APB2ENR |= (1<<9);                         // enable periperal clock for ADC1
  ADC1->SQR1  = 0x00000004;                       // only one conversion
  ADC1->SMPR2 = 0xe0000028;                       // set sample time channel1 (55,5 cycles)
  ADC1->SMPR1 = 0x00005000;                       // set sample time channel14 (55,5 cycles)
  //ADC1->SMPR1 = 0xffffffff;                       // set sample time channel14 (55,5 cycles)
  ADC1->SQR1  = 0x00000000;                       // set channel_14 as 1st conversion
  //ADC1->SQR3  = 0x00000001;                       // set channel1 as 1st conversion
  ADC1->SQR3  = 0x0000000e;                       // set channel14 as 1st conversion
  //ADC1->SQR3  = 0x00000009; 
  ADC1->CR1   = 0x00000100;                       // use independant mode, SCAN mode
  ADC1->CR2   = 0x000E0103;                       // use data align right,continuous conversion
                                                  // EXTSEL = SWSTART 
  //ADC1->CR2= 0x005E7003 ;                        // enable ADC, DMA mode, no external Trigger
  ADC1->CR2  |=  0x00500000;					  // start SW conversion
}

void mygets()
{	rx_flag = 0;
	while(rx_flag == 0)			 	
		SetEPRxValid(ENDP3);
	rx_flag = 0; }

int main(void)
{
  char zeichen;		 
  	Set_System();
  	Set_USBClock();
  	USB_Interrupts_Config();
  	USB_Init();	
	adc_Init ();																    
	lcd_init  ();	 for (i = 0; i < 10000000; i++);                 // Wait for initial display
  	lcd_clear ();
  	lcd_print (" Messgeraet");
  					/*while(rx_flag == 0) //warte auf Zeichen von USB-VCOM
  					{	  	printf("Press any key to enter!\r");    }	
					printf("%c",12);  printf("1.) i\n");	printf("2.) w\n"); */		
  while (1)
  {	zeichen = GetChar();
	switch(zeichen)
	{
		case 'i' : 	 	printf("1.) VCOM-Demobeispiel V1.1 12.7.2012\n");
			break;
		case 'w' : 	 	
		{  AD_value  = ADC_ConvertedValue;               // Read AD value
  			set_cursor(0, 1);
  			sprintf (str,"AD value = %04d", AD_value );
  			lcd_print (str);
   			printf("%04d", AD_value); }
		break;
	 default : printf("%c",7); 
	} 
  } 
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
