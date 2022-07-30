/******************************************************************************/
/* File Name:   EA_DOGM204_A.c                                                */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        18.02.2018                                                    */
/* Description: EA DOGM204-A LCD Display interfacing                          */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define EA_DOGM204_A_MOD
#include "EA_DOGM204_A.h"
#include <stm32f10x.h>

/*________________________ FUNCTION PROTOTYPES _______________________________*/

/******************************************************************************/
/*            SUBROUTINE:    init_ports()                                     */
/*                                                                            */
/* Purpose:   Initialize SPI2 and gpio-ports                                  */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void init_ports(void);

/******************************************************************************/
/*            SUBROUTINE:    LCD_DisplayOnOff(data)                           */
/*                                                                            */
/* Purpose:   Switch display on/off and configure it according to given       */
/*            data                                                            */
/* Input:     DISPLAY_ON bzw. _OFF | CURSOR_ON bzw. _OFF | BLINK_ON bzw. _OFF */
/* return:                                                                    */
/******************************************************************************/
void LCD_DisplayOnOff(char data);

/******************************************************************************/
/*            SUBROUTINE:    WriteIns(instruction)                            */
/*                                                                            */
/* Purpose:   Send instruction                                                */
/* Input:     command byte (e.g.: 0x06...bottom view)                         */
/* return:                                                                    */
/******************************************************************************/
void WriteIns(char ins);

/******************************************************************************/
/*            SUBROUTINE:    WriteData(data)                                  */
/*                                                                            */
/* Purpose:   Send data                                                       */
/* Input:     data byte (e.g.: 0x77...ASCII-Code: w -> w will be displayed)   */
/* return:                                                                    */
/******************************************************************************/
void WriteData(char data);

/******************************************************************************/
/*            SUBROUTINE:    CheckBusy()                                      */
/*                                                                            */
/* Purpose:   Checks if SPI is busy                                           */
/* Input:                                                                     */
/* return:    Content of SPI-data registers                                   */
/******************************************************************************/
char CheckBusy(void);

/******************************************************************************/
/*            SUBROUTINE:    SPI_send(byte)                                   */
/*                                                                            */
/* Purpose:   Send byte                                                       */
/* Input:     byte that should be transmittet                                 */
/* return:                                                                    */
/******************************************************************************/
void SPI_send(char byte);

/******************************************************************************/
/*            SUBROUTINE:    init_ports()                                     */
/*                                                                            */
/* Purpose:   Initialize gpio-ports                                           */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void init_ports(void)
{
  int temp;

//////////////////////////////// SPI config ////////////////////////////////////

  //Clock for SPI2
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

  //set SPI2_CR1 to
  //BD=111->f/256; CPHA=1; CPOL=1;
  //DFF=0->8b; BIDIMODE=1; BIDIOE=1; LSBFIRST=1;
  //MSTR=1; SSM=1; SSI=1; SPE=1;
  temp = SPI2->CR1;
  temp &= 0x00000000; //Set SPI2_CR1 to 0
  temp |= 0x0000C3FF;
  SPI2->CR1 = temp;

/////////////////////////////// END SPI Config ////////////////////////////////

//////////////////////////////// GPIOB config /////////////////////////////////

  //Clock for GPIOB
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;     //enable clock for GPIOB (APB2 Peripheral clock enable register)

  //MOSI,SCK set to AF_PP; MISO set to IF_PU; NSS,RESET set to GP_PP;
  temp = GPIOB->CRH;
  temp &= 0x00000000;  //Delete PB15-PB8 config bits
  temp |= 0xB4B33000;
  GPIOB->CRH = temp;

////////////////////////////// END GPIOB Config ///////////////////////////////
}

/******************************************************************************/
/*            SUBROUTINE:    Delay(ms)                                        */
/*                                                                            */
/* Purpose:   Wait for x ms                                                   */
/* Input:     Time in ms                                                      */
/* return:                                                                    */
/******************************************************************************/
void Delay (int ms)
{
  int i,j;

  for(i = 0; i < ms; i++) {
    for(j = 0; j < (8000); j++) {
    }
  }
}

/******************************************************************************/
/*            SUBROUTINE:    LCD_Init()                                       */
/*                                                                            */
/* Purpose:   Initialize LCD-Display                                          */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void LCD_Init(void)
{
  init_ports();
  NSS=1;           //NSS dauerhaft auf 1 setzen, da das &-Gatter durchschalten muss

  //Display Reset
  RESET=0;
  Delay(1);
  RESET=1;
  Delay(1);

  //init Display
  WriteIns(0x3A); //8-Bit data length extension Bit RE=1; REV=0
  WriteIns(0x09); //4 line display
  WriteIns(0x06); //Bottom view
  WriteIns(0x1E); //Bias setting BS1=1
  WriteIns(0x39); //8-Bit data length extension Bit RE=0; IS=1
  WriteIns(0x1B); //BS0=1 -> Bias=1/6
  WriteIns(0x6E); //Devider on and set value
  WriteIns(0x57); //Booster on and set contrast (BB1=C5, DB0=C4)
  WriteIns(0x72); //Set contrast (DB3-DB0=C3-C0)
  WriteIns(0x38); //8-Bit data length extension Bit RE=0; IS=0

  LCD_Clear();    //Clear display
  LCD_DisplayOnOff(DISPLAY_ON | CURSOR_ON | BLINK_ON); //turn on and configure display/cursor
}

/******************************************************************************/
/*            SUBROUTINE:    LCD_Cursor(offset)                               */
/*                                                                            */
/* Purpose:   Move cursor according to given offset                           */
/* Input:     value of offset                                                 */
/* return:                                                                    */
/******************************************************************************/
void LCD_Cursor(int anz)
{
  for(int i=1; i<=anz; i++)
  {
    LCD_WriteChar(' ');
  }
}

/******************************************************************************/
/*            SUBROUTINE:    LCD_PutChar(ASCII-Code)                          */
/*                                                                            */
/* Purpose:   Send char to display                                            */
/* Input:     Hex-value(integer) or direct-char of ASCII-character            */
/* return:                                                                    */
/******************************************************************************/
void LCD_PutChar(char ascii)
{
  WriteData(ascii);
}

/******************************************************************************/
/*            SUBROUTINE:    LCD_PutString(string)                            */
/*                                                                            */
/* Purpose:   Send string to display                                          */
/* Input:     String                                                          */
/* return:                                                                    */
/******************************************************************************/
void LCD_PutString(char string[])
{
  LCD_WriteString(string);
}

/******************************************************************************/
/*            SUBROUTINE:    LCD_WriteChar(character)                         */
/*                                                                            */
/* Purpose:   Send character to display                                       */
/* Input:     Char                                                            */
/* return:                                                                    */
/******************************************************************************/
void LCD_WriteChar (char character)
{
  WriteData(character);
}

/******************************************************************************/
/*            SUBROUTINE:    LCD_WriteString(string)                          */
/*                                                                            */
/* Purpose:   Send string to display                                          */
/* Input:     String                                                          */
/* return:                                                                    */
/******************************************************************************/
void LCD_WriteString(char *string)
{
  do
  {
    WriteData(*string++);
  }
  while(*string);
}

/******************************************************************************/
/*            SUBROUTINE:    LCD_SetPosition(postion)                         */
/*                                                                            */
/* Purpose:   Move to specific line                                           */
/* Input:     Macros: LINE1, LINE2, LINE3 or LINE4                            */
/* return:                                                                    */
/******************************************************************************/
void LCD_SetPosition(char pos)
{
  WriteIns(HOME_L1+pos);
}

/******************************************************************************/
/*            SUBROUTINE:    LCD_DisplayOnOff(data)                           */
/*                                                                            */
/* Purpose:   Switch display on/off and configure it according to given       */
/*            data                                                            */
/* Input:     DISPLAY_ON bzw. _OFF | CURSOR_ON bzw. _OFF | BLINK_ON bzw. _OFF */
/* return:                                                                    */
/******************************************************************************/
void LCD_DisplayOnOff(char data)
{
  WriteIns(0x08+data);
}

/******************************************************************************/
/*            SUBROUTINE:    LCD_Clear()                                      */
/*                                                                            */
/* Purpose:   Clear display content                                           */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
void LCD_Clear(void)
{
  WriteIns(0x01);
  LCD_SetPosition(LINE1);
}

/******************************************************************************/
/*            SUBROUTINE:    LCD_SetView(view)                                */
/*                                                                            */
/* Purpose:   Change view: bottom view, top view                              */
/* Input:     Macros: BOTTOMVIEW or TOPVIEW                                   */
/* return:                                                                    */
/******************************************************************************/
void LCD_SetView(char view)
{
  WriteIns(0x3A);
  WriteIns(view);
  WriteIns(0x38);
}

/******************************************************************************/
/*            SUBROUTINE:    LCD_SetROM(rom)                                  */
/*                                                                            */
/* Purpose:   Select ROMA, ROMB, ROMC                                         */
/*            Difference in character-set (see datasheet)                     */
/* Input:     Macros: ROMA, ROMB or ROMC                                      */
/* return:                                                                    */
/******************************************************************************/
void LCD_SetROM (char rom)
{
  WriteIns(0x3A);
  WriteIns(0x72);
  WriteData(rom);
  WriteIns(0x38);
}

/******************************************************************************/
/*            SUBROUTINE:    WriteIns(instruction)                            */
/*                                                                            */
/* Purpose:   Send instruction                                                */
/* Input:     command byte (e.g.: 0x06...bottom view)                         */
/* return:                                                                    */
/******************************************************************************/
void WriteIns(char ins)
{
  CheckBusy();
  SPI_send(0x1F);       //Send 5 synchronisation bits, RS = 0, R/W = 0
  SPI_send(ins & 0x0F);     //send lower data bits
  SPI_send((ins>>4) & 0x0F);  //send higher data bits
}

/******************************************************************************/
/*            SUBROUTINE:    WriteData(data)                                  */
/*                                                                            */
/* Purpose:   Send data                                                       */
/* Input:     data byte (e.g.: 0x77...ASCII-Code: w -> w will be displayed)   */
/* return:                                                                    */
/******************************************************************************/
void WriteData(char data)
{
  CheckBusy();
  SPI_send(0x5F);       //Send 5 synchronisation bits, RS = 1, R/W = 0
  SPI_send(data & 0x0F);    //send lower data bits
  SPI_send((data>>4) & 0x0F); //send higher data bits
}

/******************************************************************************/
/*            SUBROUTINE:    CheckBusy()                                      */
/*                                                                            */
/* Purpose:   Checks if SPI is busy                                           */
/* Input:                                                                     */
/* return:    Content of SPI-data registers                                   */
/******************************************************************************/
char CheckBusy(void)
{
  unsigned char readData = 1;

  do
  {
    SPI_send(0x3F); //Send 5 synchronisation bits, RS = 0, R/W = 1
    SPI_send(0x00); //dummy write to receive data
    while ((SPI2->SR & (SPI_SR_RXNE))); //wait while data is received
    readData= SPI2->DR; //store data
  }
  while(readData & 0x80); //check for busyflag

  return readData;
}

/******************************************************************************/
/*            SUBROUTINE:    SPI_send(byte)                                   */
/*                                                                            */
/* Purpose:   Send byte                                                       */
/* Input:     byte that should be transmittet                                 */
/* return:                                                                    */
/******************************************************************************/
void SPI_send(char byte)
{
  while (SPI2->SR & (SPI_SR_BSY));    //Abfrage, ob gerade Datenverkehr herscht
  SPI2->DR = byte;                    //Byte wird ins Data-Register des µCs geschrieben
  while (!(SPI2->SR & (SPI_SR_TXE))); //Abfrage, ob noch uebertragen wird; "!", weil waehrend der UEbertragung TXE-Flag=0
}
