/******************************************************************************/
/*   (C) Copyright Private  2009-2018 All rights reserved. AUSTRIA            */ 
/*                                                                            */ 
/* File Name:   subroutines.c                                                 */
/* Autor: 	    Berger Jonas                                                  */
/* Version: 	  V1.00                                                         */
/* Date: 		    03.05.2020                                                    */
/* Description: Unterprogramme                                                */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/

/*_______________________________ INCLUDES ___________________________________*/
#include "DS18X20.h"

/*___________________________ GLOBAL VARIABLES _______________________________*/
uint8_t datareg[9];

/*_____________________________ UNTERPROGRAMME _______________________________*/
/*******************************************************************************
Initialize Ports of STM32F103RB
--------------------------------------------------------------------------------

*******************************************************************************/
void init_ports(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //enable CLK for GPIOB
}

/*******************************************************************************
setDQasIPF(void)
--------------------------------------------------------------------------------
set DQ as IPF
*******************************************************************************/
void setDQasIPF(void)
{
	GPIOB->CRL &= ~0x0F0;               //Konfig-Bits von PB1 loeschen
	GPIOB->CRL |= 0x040;                //IPF
	/*
	GPIOB->CRL |= 0x080;                //IPF...IR=4; IPPU...IR=8,OR=1
	GPIOB->ODR |= 0x02;                 //PB1 PullUP
	*/
}

/*******************************************************************************
setDQasGPOOD(void)
--------------------------------------------------------------------------------
set DQ as GPOOD
*******************************************************************************/
void setDQasGPOOD(void)
{
	GPIOB->CRL &= ~0x0F0;               //Konfig-Bits von PB1 loeschen
	GPIOB->CRL |= 0x0F0;                //GPOOD...IR=7; AFOOD...IR=F; AFOPP...IR=B
}

/*******************************************************************************
Send Master-Reset-Impuls
--------------------------------------------------------------------------------
This function will release the master reset impuls and checks if there is at
least one 1-wire-component connected to the bus (0) or not (1).
*******************************************************************************/
uint8_t sendMRI(void)
{
	uint8_t rec;
	
	setDQasGPOOD();
  DQ_o = 0;							 // DQ to 0 for MRI
  waitus(490);           // wait for >480 us
  DQ_o = 1;              // config the  DQ-IO as input (high-z -> pull up)
  setDQasIPF();
	waitus(40);
	rec = DQ_i;            // read the level (if low, slave available)
  waitus(450);           // wait for end of slot
  
	return (rec);
}

/*******************************************************************************
 Write_Bit
 -------------------------------------------------------------------------------
 This function will write one single bit on the bus.
 *******************************************************************************/
void writeBit (uint8_t val)
{
	setDQasGPOOD();        // set the (I)O to low level
  DQ_o = 0;              // config the DQ-IO as output (-> low)
	if(val)                // if the bit to transfer is a "1"
  {
    waitus(1);           // wait 1 us and..
    DQ_o = 1;            // ..config the DQ-IO as input (high-z -> pull up)
  }
  waitus(100);           // wait for end of slot
	DQ_o = 1;              // config the DQ-IO as input (high-z -> pull up)
}

/*******************************************************************************
 Write_Byte
 -------------------------------------------------------------------------------
 This function will write a complete byte on the bus.
 *******************************************************************************/
void writeByte (uint8_t val)
{
  uint8_t i, mask = 1;
    
	// write the byte by sending eight bits (LSB first)
  for (i=0; i<8; i++)
  {
		writeBit(val & mask);
    mask = (mask << 1);
  }
}

/*******************************************************************************
 Read_Bit
 -------------------------------------------------------------------------------
 This function will read one single bit from the bus.
 *******************************************************************************/
uint8_t readBit (void)
{
	uint8_t rec;           
  
	setDQasGPOOD();
	DQ_o = 0;           // perform a very short low impuls
	DQ_o = 1;           // config the DQ-IO as input (high-z -> pull up)
	setDQasIPF();
  waitus(15);
  rec = DQ_i;              // read the level on DQ (this is the read bit)
  waitus(105);       // wait for end of slot
    
	return(rec);
}

/*******************************************************************************
 Read_Byte
 --------------------------------------------------------------------------------
 This function will read a complete byte from the bus.
 *******************************************************************************/
uint8_t readByte(void)
{
  uint8_t value = 0 , i;
	
  // read the byte by reading eight bits (LSB first)
  for(i=0;i<8;i++)
  {
		if (readBit())
    {
			value |= 0x01 << i;
    }
  }
  return(value);
}

/*******************************************************************************
Skip_Rom
--------------------------------------------------------------------------------
This function sends the skip rom command and calls all connected components.
*******************************************************************************/

void skipROM(void)
{
	writeByte(0xCC);
}

/*******************************************************************************
Convert Temperature
--------------------------------------------------------------------------------
convert sensordata to temperature
*******************************************************************************/
float convTemp(uint8_t val[])
{
	float ftemp; // Temperatur als float-Var.
  // Die Scratch Pad Werte sind im globalen Array ´datareg[ ]´
  // abgelegt
	
  // Temperatur berechnen, als float-Wert
  if(datareg[1] == 0) // Positive Temp.-Werte
  {
	  ftemp = datareg[0]/2.0;
  }
  else // Neg. Temp.-Werte
  {
	  ftemp = (~datareg[0])+1; // 2er-Komplement
	  ftemp = (ftemp*(-1))/2;
  }
	
	return ftemp;
}
