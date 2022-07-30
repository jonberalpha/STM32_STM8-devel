/******************************************************************************/
/* File Name:   SENS_DS18X20.c                                                */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        15.06.2020                                                    */
/* Description: Get temperature of the 1-wire-sensor                          */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define SENS_DS18X20_MOD

/*_______________________________ INCLUDES ___________________________________*/
#include "SENS_DS18X20.h"

/*________________________________ EXTERN ____________________________________*/
extern uint8_t datareg[9];

/*___________________________ GLOBAL VARIABLES _______________________________*/
uint8_t datareg[9];

/*__________________________ FUNCTION PROTOTYPES _____________________________*/
static void init_ports(void);
static uint8_t send_mri(void);
static uint8_t read_bit(void);
static uint8_t read_byte(void);
static void write_bit(uint8_t val);
static void write_byte(uint8_t val);
static void skip_ROM(void);
static float conv_temp(void);

/*______________________________ SUBROUTINES _________________________________*/
/******************************************************************************/
/*            SUBROUTINE:    init_ports()                                     */
/*                                                                            */
/* Purpose:   initializes Port configuration of STM32F10X                     */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
static void init_ports(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //enable CLK for GPIOB
  GPIOB->CRL &= ~0x0F;               //Konfig-Bits von PB1 loeschen
  GPIOB->CRL |= 0x07;                //GPOOD...IR=7; AFOOD...IR=F; AFOPP...IR=B
}

/******************************************************************************/
/*            SUBROUTINE:    DS18X20_init()                                   */
/*                                                                            */
/* Purpose:   initializes DS18X20 Environment                                 */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void DS18X20_init(void)
{
  init_ports();
  wait_init();
}

/******************************************************************************/
/*            SUBROUTINE:    send_mri()                                       */
/*                                                                            */
/* Purpose:   Send Master-Reset-Impuls                                        */
/*            This function will release the master reset impuls and checks   */
/*            if there is at least one 1-wire-component connected             */
/*            to the bus (0) or not (1).                                      */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
static uint8_t send_mri(void)
{
  uint8_t rec;

  DQ_o = 0;              // DQ to 0 for MRI
  wait_us(490);          // wait for >480 us
  DQ_o = 1;              // config the  DQ-IO as input (high-z -> pull up)
  wait_us(40);
  rec = DQ_i;            // read the level (if low, slave available)
  wait_us(450);          // wait for end of slot

  return rec;
}

/******************************************************************************/
/*            SUBROUTINE:    write_bit()                                      */
/*                                                                            */
/* Purpose:   Write_Bit                                                       */
/*            This function will write one single bit on the bus.             */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
static void write_bit(uint8_t val)
{
  DQ_o = 0;              // config the DQ-IO as output (-> low)
  if(val)                // if the bit to transfer is a "1"
  {
    wait_us(1);           // wait 1 us and..
    DQ_o = 1;            // ..config the DQ-IO as input (high-z -> pull up)
  }
  wait_us(100);           // wait for end of slot
  DQ_o = 1;              // config the DQ-IO as input (high-z -> pull up)
}

/******************************************************************************/
/*            SUBROUTINE:    write_byte()                                     */
/*                                                                            */
/* Purpose:   Write_Byte                                                      */
/*            This function will write a complete byte on the bus.            */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
static void write_byte(uint8_t val)
{
  uint8_t i, mask = 1;

  // write the byte by sending eight bits (LSB first)
  for (i=0; i<8; i++)
  {
    write_bit(val & mask);
    mask = (mask << 1);
  }
}

/******************************************************************************/
/*            SUBROUTINE:    read_byte()                                      */
/*                                                                            */
/* Purpose:   Read_Bit                                                        */
/*            This function will read one single bit from the bus.            */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
static uint8_t read_bit (void)
{
  uint8_t rec;

  DQ_o = 0;           // perform a very short low impuls
  DQ_o = 1;           // config the DQ-IO as input (high-z -> pull up)
  wait_us(15);
  rec = DQ_i;         // read the level on DQ (this is the read bit)
  wait_us(105);       // wait for end of slot

  return rec;
}

/******************************************************************************/
/*            SUBROUTINE:    read_byte()                                      */
/*                                                                            */
/* Purpose:   Read_Byte                                                       */
/*            This function will read a complete byte from the bus.           */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
static uint8_t read_byte(void)
{
  uint8_t value = 0, i;

  // read the byte by reading eight bits (LSB first)
  for(i=0; i<8; i++)
  {
    if(read_bit())
    {
      value |= 0x01 << i;
    }
  }
  return value;
}

/******************************************************************************/
/*            SUBROUTINE:    skip_ROM()                                       */
/*                                                                            */
/* Purpose:   Skip_Rom                                                        */
/*            This function sends the skip rom command and calls all          */
/*            connected components.                                           */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
static void skip_ROM(void)
{
  write_byte(SKIP_ROM);
}

/******************************************************************************/
/*            SUBROUTINE:    conv_temp()                                      */
/*                                                                            */
/* Purpose:   Convert Temperature                                             */
/*            convert sensordata to temperature                               */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
static float conv_temp()
{
  uint8_t sign = datareg[1] & 0x80;
  int16_t temp = (datareg[1] << 8) + datareg[0];

  if(sign)
  {
    temp = ((temp ^ 0xffff) + 1) * -1;
  }

  return temp/16.0;
}

/******************************************************************************/
/*            SUBROUTINE:    get_temperature()                                */
/*                                                                            */
/* Purpose:   Get Temperature                                                 */
/*            whole Process to get Temperature                                */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
float get_temperature()
{
  int i=0;
  float temp=0;

  send_mri();       //send Master Reset Impulse; must return 0x00
  skip_ROM();              //simply calls all slaves instead of an specific address call

  write_byte(CONVERT_TEMP); //Temperaturmessung ausfuehren
  wait_ms(750);             //Messdauer abwarten

  send_mri();
  skip_ROM();

  write_byte(READ_SCRATCHPAD); //Temperaturwert auslesen

  for(i=0; i<9; i++)  //Antwort einlesen: 9 Byte große Scratch Pad-Inhalt einlesen
  {
    datareg[i] = read_byte();
  }
  temp = conv_temp();

  return temp;
}
