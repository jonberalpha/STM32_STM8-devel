/******************************************************************************/
/* File Name:   ONEWIRE_STM32F10X.c                                           */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        10.07.2020                                                    */
/* Description: 1-wire Library                                                */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define ONEWIRE_STM32F10X_MOD

/*_______________________________ INCLUDES ___________________________________*/
#include "ONEWIRE_STM32F10X.h"

/*______________________________ STRUCTURES __________________________________*/
typedef struct OneWire
{
  GPIO_TypeDef *GPIOx;
  uint32_t GPIOx_Base;
  uint32_t RCC_REG;
  volatile unsigned long *DQ_o;
  volatile unsigned long *DQ_i;
} OneWire;

/*___________________________ GLOBAL VARIABLES _______________________________*/
OneWire ow;

/*__________________________ FUNCTION PROTOTYPES _____________________________*/
static void init_port(int pin);

/*_____________________________ SUBROUTINES __________________________________*/
/******************************************************************************/
/*            SUBROUTINE:    ow_init()                                        */
/*                                                                            */
/* Purpose:   initializes Setup                                               */
/* Input:     GPIOA-GPIOE, 0-15                                               */
/* return:                                                                    */
/******************************************************************************/
void ow_init(GPIO_TypeDef *GPIOx,int pin)
{
  uint32_t GPIOx_ODR,GPIOx_IDR;

  //handle Bitbanding
  if(GPIOx == GPIOA)
  {
    ow.GPIOx_Base = GPIOA_BASE;
    ow.RCC_REG = RCC_APB2ENR_IOPAEN;
  }
  else if(GPIOx == GPIOB)
  {
    ow.GPIOx_Base = GPIOB_BASE;
    ow.RCC_REG = RCC_APB2ENR_IOPBEN;
  }
  else if(GPIOx == GPIOC)
  {
    ow.GPIOx_Base = GPIOC_BASE;
    ow.RCC_REG = RCC_APB2ENR_IOPCEN;
  }
  else
  {
    ow.GPIOx_Base = GPIOD_BASE;
    ow.RCC_REG = RCC_APB2ENR_IOPDEN;
  }

  ow.GPIOx = GPIOx;

  GPIOx_ODR = ow.GPIOx_Base + 3*sizeof(uint32_t); //Calc peripheral address GPIOx ODR
  GPIOx_IDR = ow.GPIOx_Base + 2*sizeof(uint32_t); //Calc peripheral address GPIOx IDR

  //calc Pointer and assign it to DQ-line in the OneWire Structure using BitBanding
  ow.DQ_o = (volatile unsigned long *) ((PERIPH_BB_BASE + (GPIOx_ODR-PERIPH_BASE)*32 + (pin*4))); //DQ out
  ow.DQ_i = (volatile unsigned long *) ((PERIPH_BB_BASE + (GPIOx_IDR-PERIPH_BASE)*32 + (pin*4))); //DQ in

  //initialize Ports and Timer
  init_port(pin);
  wait_init();
}

/******************************************************************************/
/*            SUBROUTINE:    init_port()                                      */
/*                                                                            */
/* Purpose:   setup Port and Timer of CM3                                     */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
static void init_port(int pin)
{

  RCC->APB2ENR |= ow.RCC_REG; //enable CLK for GPIOx

  if(pin < 8)
  {
    ow.GPIOx->CRL &= (~0xF << (pin*4));   //Konfig-Bits von GPIOx loeschen
    ow.GPIOx->CRL |= (0x7 << (pin*4));    //GPOOD...IR=7;
  }
  else
  {
    ow.GPIOx->CRH &= (~0xF << ((pin-8)*4)); //Konfig-Bits von PB1 loeschen
    ow.GPIOx->CRH |= (0x7 << ((pin-8)*4));  //GPOOD...IR=7;
  }
}

/******************************************************************************/
/*            SUBROUTINE:    ow_write_bit()                                   */
/*                                                                            */
/* Purpose:   Write Bit                                                       */
/*            This function will write one single bit on the bus.             */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void ow_write_bit(uint8_t val)
{
  *ow.DQ_o = 0;           // config the DQ-IO as output (-> low)
  if(val)                 // if the bit to transfer is a "1"
  {
    wait_us(1);           // wait 1 us and..
    *ow.DQ_o = 1;         // ..config the DQ-IO as input (high-z -> pull up)
  }
  wait_us(100);           // wait for end of slot
  *ow.DQ_o = 1;           // config the DQ-IO as input (high-z -> pull up)
}

/******************************************************************************/
/*            SUBROUTINE:    ow_write_byte()                                  */
/*                                                                            */
/* Purpose:   Write Byte                                                      */
/*            This function will write a complete byte on the bus.            */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void ow_write_byte(uint8_t val)
{
  uint8_t i, mask = 1;

  // write the byte by sending eight bits (LSB first)
  for (i=0; i<8; i++)
  {
    ow_write_bit(val & mask);
    mask = (mask << 1);
  }
}

/******************************************************************************/
/*            SUBROUTINE:    ow_read_bit()                                    */
/*                                                                            */
/* Purpose:   Read Bit                                                        */
/*            This function will read one single bit from the bus.            */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
uint8_t ow_read_bit()
{
  uint8_t rec;

  *ow.DQ_o = 0;       // perform a very short low impuls
  *ow.DQ_o = 1;       // config the DQ-IO as input (high-z -> pull up)
  wait_us(15);
  rec = *ow.DQ_i;     // read the level on DQ (this is the read bit)
  wait_us(105);       // wait for end of slot

  return rec;
}

/******************************************************************************/
/*            SUBROUTINE:    ow_read_byte()                                   */
/*                                                                            */
/* Purpose:   Read Byte                                                       */
/*            This function will read a complete byte from the bus.           */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
uint8_t ow_read_byte()
{
  uint8_t value = 0, i;

  // read the byte by reading eight bits (LSB first)
  for(i=0; i<8; i++)
  {
    if(ow_read_bit())
    {
      value |= 0x01 << i;
    }
  }
  return value;
}

/******************************************************************************/
/*            SUBROUTINE:    ow_send_mri()                                    */
/*                                                                            */
/* Purpose:   Send Master-Reset-Impuls                                        */
/*            This function will release the master reset impuls and checks   */
/*            if there is at least one 1-wire-component connected             */
/*            to the bus (0) or not (1).                                      */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
uint8_t ow_send_mri(void)
{
  uint8_t rec;

  *ow.DQ_o = 0;          // DQ to 0 for MRI
  wait_us(490);          // wait for >480 us
  *ow.DQ_o = 1;          // config the  DQ-IO as input (high-z -> pull up)
  wait_us(40);
  rec = *ow.DQ_i;        // read the level (if low, slave available)
  wait_us(450);          // wait for end of slot

  return rec;
}
