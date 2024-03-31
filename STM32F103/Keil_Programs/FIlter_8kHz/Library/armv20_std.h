/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2010 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   armv20_std.h                                                  */
/* Autor:       Josef Reisinger                                               */
/* Version:     V1.00                                                         */
/* Date:        19/05/2012                                                    */
/* Description: Standard Library für ARM Corttex M3                           */
/******************************************************************************/
/* History: V1.00  creation                                                   */
/******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ARMV20_STD_H
#define __ARMV20_STD_H

/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include <stdio.h>

/*----------------------------- Define Pins for LED and Swistches--------------*/
/* Einzel-Portpins im Bitbandbereich festlegen                                 */

#define GPIOA_IDR GPIOA_BASE + 2*sizeof(uint32_t)    
#define GPIOA_ODR GPIOA_BASE + 3*sizeof(uint32_t)
#define GPIOB_IDR GPIOB_BASE + 2*sizeof(uint32_t)   
#define GPIOB_ODR GPIOB_BASE + 3*sizeof(uint32_t)   
#define GPIOC_IDR GPIOC_BASE + 2*sizeof(uint32_t)   
#define GPIOC_ODR GPIOC_BASE + 3*sizeof(uint32_t)
#define GPIOD_IDR GPIOD_BASE + 2*sizeof(uint32_t)    
#define GPIOD_ODR GPIOD_BASE + 3*sizeof(uint32_t)
#define GPIOE_IDR GPIOE_BASE + 2*sizeof(uint32_t)    
#define GPIOE_ODR GPIOE_BASE + 3*sizeof(uint32_t)


// Calc Bit Band Adress from peripheral address: a = peripheral address b = Bit number
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))  

#define SW0  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,8)))  // PE0
#define SW1  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,9)))  // PE1
#define SW2  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,10)))  // PE2
#define SW3  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,11)))  // PE3
#define SW4  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,12)))  // PE4
#define SW5  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,13)))  // PE5
#define SW6  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,14)))  // PE6
#define SW7  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_IDR,15)))  // PE7

#define PIEZO *((volatile unsigned long *)(BITBAND_PERI(GPIOA_ODR,0)))  //Portleitung des Piezo-Summers (PA0)

#define LED0  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,0)))  // PE0
#define LED1  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,1)))  // PE1
#define LED2  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,2)))  // PE2
#define LED3  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,3)))  // PE3
#define LED4  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,4)))  // PE4
#define LED5  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,5)))  // PE5
#define LED6  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,6)))  // PE6
#define LED7  *((volatile unsigned long *)(BITBAND_PERI(GPIOE_ODR,7)))  // PE7

/* ---------------------Vordefinierte Frequenzen fuer Tonerzeugung -----------*/
#define ton_C1 262
#define ton_D1 293
#define ton_E1 329
#define ton_F1 349
#define ton_G1 391
#define ton_A1 440
#define ton_H1 493
#define ton_C2 523
#define ton_D2 587
#define ton_E2 659
#define ton_F2 698
#define ton_G2 783 
#define ton_A2 880
#define ton_H2 987
#define ton_C3 1046

/* ------------------- Vordefinierte Längen (ms) für Tonerzeugung --------------*/
#define ganze    800
#define halbe    400
#define viertel   200
#define achtel    100
#define sechzehntel  50

/*--------------------------------- Definitions LCD --------------------------*/
#define LineLen     16                  /* Width of LCD (in characters)       */
#define NumLines     2                  /* Hight of LCD (in lines)            */

#ifdef ARMV20_STD_MOD
#define EXPORT
#else
#define EXPORT extern
#endif 

/* ----------------------------Exported functions ---------------------------- */

/* ------------------------- Miscellaneous Functions --------------------------*/

/******************************************************************************/
/*            U N T E R P R O G R A M M:                                      */
/*                                                                            */
/* Aufgabe: Function converts a Nibble(0-F) to an ASCII ('0'-'F')             */
/* Input:   nib: Nibble to convert                                            */
/* Output:  nib: Converted Nibble                                             */ 
/* return:  -                                                                 */
/******************************************************************************/
EXPORT void nib2asc(char *nib);


/******************************************************************************/
/*            U N T E R P R O G R A M M:                                      */
/*                                                                            */
/* Aufgabe: Function converts an ASCII('0'-'F') to a Nibble(0-F)              */
/* Input:   asc: ASCII Code                                                   */
/* Output:  asc: Hex Value                                                    */ 
/* return:  -                                                                 */
/******************************************************************************/
EXPORT void asc2nib(char *asc);

/******************************************************************************/
/*            U N T E R P R O G R A M M:                                      */
/*                                                                            */
/* Aufgabe: Function converts a Hex-Code (00-FF) to an BCD (0-255)            */
/* Input:   hex: Hex Value                                                    */
/* return:  BCD Value                                                         */
/******************************************************************************/
EXPORT int hex2bcd(char hex);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    WAIT_MS                           */
/*                                                                            */
/* Aufgabe:   Wartet die angegebene Anzahl an Millisekunden                   */
/* Input:     ms = Anzahl der zu wartenden Millisekunden                      */
/* return:                                                                   */
/******************************************************************************/
EXPORT void wait_ms(int ms);

/******************************************************************************/
/*            U N T E R P R O G R A M M:    wait_10us                         */
/*                                                                            */
/* Aufgabe:   Wartet 10µs                                                     */
/* Input:                                                                     */
/* return:                                                                   */
/******************************************************************************/
EXPORT void wait_10us(void);


/* ------------------------- LED/ Switches Functions -------------------*/

/******************************************************************************/
/*            U N T E R P R O G R A M M:    init_leds_switches                */
/*                                                                            */
/* Aufgabe:   Initialisiert Portleitungen für LED / Schalterplatine           */
/* Input:                                                                     */
/* return:                                                                    */
/******************************************************************************/
EXPORT void init_leds_switches(void); 


/*******************************************************************************/
/*            U N T E R P R O G R A M M:    set_leds                          */
/*                                                                            */
/* Aufgabe:  gibt hexadezimalen Wert in auf 8 Leds aus                        */
/*           (an Port 0 angeschlossen)                                        */
/* Input:    value = Wert auf den LEDS gesetzt werden sollen                  */                                                               
/* return:                                                                    */
/******************************************************************************/
EXPORT void set_leds (char value);


/******************************************************************************/
/*            U N T E R P R O G R A M M:    get_switches                      */
/*                                                                            */
/* Aufgabe:   liest aktuelle Schalterstellung ein                             */
/* Input:                                                                     */
/* return:    aktuelle Schalterstellung                                       */
/******************************************************************************/
EXPORT char get_switches(void);


/* ------------------------- UART Functions ----------------------------------*/

/******************************************************************************/
/*            U N T E R P R O G R A M M:                                      */
/*                                                                            */
/* Aufgabe: Initialisiert UART2 (Remapped Port Pins)                          */
/* Input:   Baudrate                                                          */
/* return:                                                                   */
/******************************************************************************/
EXPORT void uart_init(unsigned long baudrate);

/******************************************************************************/
/*            U N T E R P R O G R A M M:                                      */
/*                                                                            */
/* Aufgabe: liest ein Zeichen von UART1 ein                                   */
/* Input:                                                                     */
/* return:  eingelesens Zeichen                                               */
/******************************************************************************/
EXPORT char uart_get_char(void); 

/******************************************************************************/
/*            U N T E R P R O G R A M M:                                      */
/*                                                                            */
/* Aufgabe: Ausgabe eines Zeichens auf UART1                                  */
/* Input:   ch: Zeichen in ASCII Code                                         */
/* return:                                                                   */
/******************************************************************************/
EXPORT void uart_put_char(char ch);

/******************************************************************************/
/*            U N T E R P R O G R A M M:                                      */
/*                                                                            */
/* Aufgabe: Ausgabe eines 8 Bit Hex Wertes als ASCII String auf UART1         */
/* Input:                                                                     */
/* return:                                                                   */
/******************************************************************************/
EXPORT void uart_put_hex(char c); 

/******************************************************************************/
/*            U N T E R P R O G R A M M:                                      */
/*                                                                            */
/* Aufgabe: Ausgabe eines Strings auf UART1                                   */
/* Input:   string: C- String der aud UART1 ausgegeben werden soll            */
/* return:                                                                   */
/******************************************************************************/
EXPORT void uart_put_string(char *string);

/******************************************************************************/
/*            U N T E R P R O G R A M M:                                      */
/*                                                                            */
/* Aufgabe: sendet ein CLEAR Screen String ("ESC[2J") zu einem VT100 Terminal */
/* Input:                                                                     */
/* return:                                                                   */
/******************************************************************************/
EXPORT void uart_clear(void);

/******************************************************************************/
/*            U N T E R P R O G R A M M:                                      */
/*                                                                            */
/* Aufgabe:  sendet Positionierungsstring auf VT100 Terminal "ESC[y;xH"       */
/* Input:                                                                     */
/* return:                                                                   */
/******************************************************************************/
EXPORT void uart_setpos(char x,char y);

/* ------------------------- LCD Functions ----------------------------------*/

/*******************************************************************************
* Initialize the LCD controller                                                *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/
EXPORT void lcd_init (void);

/*******************************************************************************
* Write command to LCD controller                                              *
*   Parameter:    c:      command to be written                                *
*   Return:                                                                    *
*******************************************************************************/
EXPORT void lcd_write_cmd (unsigned char c);

/*******************************************************************************
* Write data to LCD controller                                                 *
*   Parameter:    c:      data to be written                                   *
*   Return:                                                                    *
*******************************************************************************/
EXPORT void lcd_write_data (unsigned char c);

/******************************************************************************/
/*            U N T E R P R O G R A M M:                                      */
/*                                                                            */
/* Aufgabe: Print Character to current cursor position                        */
/* Input:   c:      character to be printed                                   */
/* return:                                                                   */
/******************************************************************************/
EXPORT void lcd_put_char (char c);

/******************************************************************************/
/*            U N T E R P R O G R A M M:                                      */
/*                                                                            */
/* Aufgabe: Ausgabe eines 8 Bit Hex Wertes als ASCII String auf LCD           */
/* Input:   c: Hex Value to be printed                                        */
/* return:                                                                   */
/******************************************************************************/
EXPORT void lcd_put_hex(char c); 


/*******************************************************************************/
/* Set cursor position on LCD display                                          */
/*   Parameter: line:   line position (0-3)                                    */
/*              column: column position (0-15)                                 */
/*   Return:                                                                   */
/*******************************************************************************/
EXPORT void lcd_set_cursor (int line_nr, int column_nr);

/*******************************************************************************
* Clear the LCD display                                                        *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/
EXPORT void lcd_clear (void);

/*******************************************************************************
* Print sting to LCD display                                                   *
*   Parameter:    string: pointer to output string                             *
*   Return:                                                                    *
*******************************************************************************/
EXPORT void lcd_put_string (char *string);

/*******************************************************************************
* Print a bargraph to LCD display                                              *
*   Parameter:     val:  value 0..100 %                                        *
*                  size: size of bargraph 1..16                                *
*   Return:                                                                    *
*******************************************************************************/
EXPORT void lcd_bargraph (int value, int size);

/*******************************************************************************
* Display bargraph on LCD display                                              *
*   Parameter:     pos_y: vertical position of bargraph                        *
*                  pos_x: horizontal position of bargraph start                *
*                  value: size of bargraph active field (in pixels)            *
*   Return:                                                                    *
*******************************************************************************/
EXPORT void lcd_bargraphXY (int pos_y, int pos_x, int value);

/* ------------------------- ADC Functions ----------------------------------*/

/******************************************************************************/
/*            U N T E R P R O G R A M M:    adc1_convert                      */
/*                                                                            */
/* Aufgabe:  liefert aktuellen Wert von ADC1 für Kanal channel                */
/*                                                                            */
/* Input:  channel to convert                                                 */                                                               
/* return:  converted value (12Bit right aligned)                             */
/******************************************************************************/
EXPORT unsigned short int adc1_convert(unsigned char channel);


/******************************************************************************/
/*            U N T E R P R O G R A M M:    ton                               */
/*                                                                            */
/* Aufgabe:  Wiedergabe eines Ton auf Piezo Summer                            */
/*                                                                            */
/* Input:   frequenz: Frequenz des Tons in Hz                                 */                                                               
/*          dauer:    Dauer des in ms                                         */
/* return:                                                                    */
/******************************************************************************/
EXPORT void ton(int frequenz,int dauer_ms);


#undef  EXPORT
#endif /* __ARMV20_STD_H */

/******************* (C) HTL - HOLLABRUNN  2009-2010 *****END OF FILE****/
