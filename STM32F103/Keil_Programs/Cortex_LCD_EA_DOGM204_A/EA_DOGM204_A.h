/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   EA_DOGM204_A.h                                                */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        18.02.2018                                                    */
/* Description: EA DOGM204-A LCD Display ansprechen                           */
/*              Header-File stellt defines und Prototypen zur Verfuegung      */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#ifndef EA_DOGM204_A_H_INCLUDED
#define EA_DOGM204_A_H_INCLUDED

/* -----------------------------------Includes -------------------------------*/
#include <stm32f10x.h>

#ifdef  EA_DOGM204_A_MOD
#define EXPORT
#else
#define EXPORT extern
#endif 

/* ----------------------------Exported functions --------------------------- */

/*___________________________ FUNCTION PROTOTYPES_____________________________*/

/******************************************************************************/
/*            U N T E R P R O G R A M M:    init_ports()                      */
/*                                                                            */
/* Aufgabe:   Initialisiert µC-internes SPI2 und die Portleitungen            */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void init_ports(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    Delay(ms)                         */
/*                                                                            */
/* Aufgabe:   Wartet den angegebenen Wert in ms                               */
/* Input:     Integer-Zahl in ms                                              */
/* return:                                                                    */
/******************************************************************************/
EXPORT void Delay(int);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    LCD_Init()                        */
/*                                                                            */
/* Aufgabe:   LCD-Display initialisieren                                      */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_Init(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    LCD_PutChar(ASCII-Code)           */
/*                                                                            */
/* Aufgabe:   Sendet den angegebenen ASCII-Code an das Display                */
/* Input:     ASCII-Code in Hex-Wert                                          */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_PutChar(char ascii);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    LCD_PutString(ASCII-Code Kette)   */
/*                                                                            */
/* Aufgabe:   Sendet eine Reihe von ASCII-Codes an das Display                */
/* Input:     Einzelne ASCII-Codes in Hex-Wert                                */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_PutString(char string[]);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    LCD_Cursor(anzahl)                */
/*                                                                            */
/* Aufgabe:   Verschiebt den Cursor um den angegebenen Wert                   */
/* Input:     Integer Wert fuer die Anzahl                                    */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_Cursor(int anz);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    LCD_Clear()                       */
/*                                                                            */
/* Aufgabe:   Displayinhalt loeschen und Cursor in die 1.Zeile stellen        */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_Clear(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    LCD_SetPosition(postion)          */
/*                                                                            */
/* Aufgabe:   Auswahl der Zeile in die geschrieben werden soll                */
/* Input:     Entweder LINE1, LINE2, LINE3 oder LINE4                         */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_SetPosition(char pos);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    LCD_DisplayOnOff(data)            */
/*                                                                            */
/* Aufgaben:  Display ein- und ausschalten bzw.                               */
/*            Cursor ein- und auschalten bzw.                                 */
/*            Cursor-Blinken ein- und auschalten                              */
/*            Input muss mit Trennstrichen ("|") geschrieben werden           */
/* Input:     DISPLAY_ON bzw. _OFF | CURSOR_ON bzw. _OFF | BLINK_ON bzw. _OFF */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_DisplayOnOff(char data);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    LCD_SetView(view)                 */
/*                                                                            */
/* Aufgabe:   Darstellung bzw. Drehen der Anzeige: bottom view, top view      */
/* Input:     Entweder BOTTOMVIEW oder TOPVIEW                                */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_SetView(char view);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    LCD_SetROM(rom)                   */
/*                                                                            */
/* Aufgabe:   Auswahl des Zeichensatzes (ROMA, ROMB, ROMC)                    */
/*            Der Unterschied liegt im jew. ROM gespeicherten Zeichensatz     */
/*            (siehe Datenblatt)                                              */
/* Input:     ROMA, ROMB, ROMC                                                */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_SetROM(char rom);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    LCD_WriteChar(character)          */
/*                                                                            */
/* Aufgabe:   Sendet direkt ein Character an das Display                      */
/* Input:     Beliebiges Character                                            */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_WriteChar(char character);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    LCD_WriteString(string)           */
/*                                                                            */
/* Aufgabe:   Sendet direkt einen String an das Display                       */
/* Input:     Beliebiger String                                               */
/* return:                                                                    */
/******************************************************************************/
EXPORT void LCD_WriteString(char *string);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    WriteIns(instruction)             */
/*                                                                            */
/* Aufgabe:   Sendet anfangs den HEX-Wert 0x1F als Start-Byte.                */
/*            Damit erkennt das Display, dass es sich um eine Instruction und */
/*            nicht um einen Datensatz handelt.                               */
/*            Danach wird der Befehl an das Display gesendet                  */
/* Input:     Befehlsbyte (z.B.: 0x06...bottom view)                          */
/* return:                                                                    */
/******************************************************************************/
EXPORT void WriteIns(char ins);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    WriteData(data)                   */
/*                                                                            */
/* Aufgabe:   Sendet anfangs den HEX-Wert 0x5F als Start-Byte.                */
/*            Damit erkennt das Display, dass es sich um einen Datensatz und  */
/*            nicht um eine Instruction handelt.                              */
/*            Danach wird der Datensatz an das Display gesendet               */
/* Input:     Datenbyte (z.B.: 0x77...ASCII-Code: w -> w erscheint am Display)*/
/* return:                                                                    */
/******************************************************************************/
EXPORT void WriteData(char data);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    CheckBusy()                       */
/*                                                                            */
/* Aufgabe:   Abfrage, ob gelesen wird (Read aktiv oder nicht?)               */
/* Input:                                                                     */
/* return:    Inhalt des Data Registers des µCs                               */
/******************************************************************************/
EXPORT char CheckBusy(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    SPI_send(byte)                    */
/*                                                                            */
/* Aufgabe:   Grundbestandteil der SPI-UEbertragung                           */
/*            Sendet das angegebene Byte in LSBFirst-Format an das Display    */
/* Input:     Zu uebertragenes Byte                                           */
/* return:                                                                    */
/******************************************************************************/
EXPORT void SPI_send(char byte);

/*______________________________ BITBANDING __________________________________*/
#define GPIOB_ODR GPIOB_BASE + 3*sizeof(uint32_t) //Calc peripheral address GPIOB ODR
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))  
#define RESET  *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,11))) //RESET/PB11
#define NSS   *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,12)))   //NSS/PB12

/*___________________________ CONSTANT DEFINITION ____________________________*/
#define HOME_L1      0x80
#define LINE1        0
#define LINE2        LINE1+0x20
#define LINE3        LINE1+0x40
#define  LINE4       LINE1+0x60

#define DISPLAY_ON   0x04
#define DISPLAY_OFF 0x03
#define CURSOR_ON    0x02
#define CURSOR_OFF  0x05
#define BLINK_ON    0x01
#define BLINK_OFF    0x06

#define TOPVIEW      0x05
#define BOTTOMVIEW  0x06
#define ROMA        0x00
#define ROMB        0x04
#define ROMC        0x0C

#undef  EXPORT
#endif /* __EA_DOGM204_A_H */

/******************* (C) HTL - HOLLABRUNN  2009-2010 *****END OF FILE****/
