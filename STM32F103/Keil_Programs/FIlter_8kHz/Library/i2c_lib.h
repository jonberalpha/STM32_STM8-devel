#ifndef  I2C_H     //keine Mehrfach definition
#define  I2C_H

#include "stm32f10x.h"
/********************************************************************************************************************************/
/* \ *\     I 2 C - L I B R A R Y                                          23.07.2010                                      \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    HTL Hollabrunn   AUSTRIA                                                                                        \* / */
/* / *\    Author:          Maier Jakob                                                                                    \* \ */
/* \ *\    Date:            März 2011                                                                                      \* / */
/* / *\    Version:         2.0                                                                                            \* \ */
/* \ *\    Description:  Beinhaltet alle wesentlichen Unterprogramme die zur Kommunikation über den I2C-Bus                \* / */
/* / *\                  als Master notwendig sind                                                                         \* \ */
/* \ *\    (c) Maier Jakob                                                                                                 \* / */
/********************************************************************************************************************************/

//Versionshinweise:
//*****************
//
//Ver. 1.0:  Funktionen für I2C1
//Ver. 2.0:  Erweitert für I2C1 und I2C2, in I2C_error_message kann jeder UART gewählt werden

//Hardware:
//*********
//
//I2C1: SCL=PB6
//      SDA=PB7
//I2C2: SCL=PB10
//      SDA=PB11
//
//Wichtig: Die Leitungen zur Sicherheit mit 2stk. 4,7kOhm Pull-Up Widerständen an die 3,3V anpassen!
//


 //Master Receiver:
 //****************
 //
 //1 Byte + Stop:  init (-) start(adr+1,1)  -  read(&dat,x)
 //1 Byte + Stop:  init (-)  I2Cx_read_1(adr+x,&dat)
 //
 //2 Byte + Stop:  init (-) start(adr+1,0)  -  read(&dat,1)  -  read(&dat,x)
 //2 Byte + Stop:  init (-) start(adr+1,0)  -  I2Cx_read_last2(&dat1,&dat2)
 //
 //3 Byte + Stop:  init (-) start(adr+1,0) (-) read(&dat,0)  -  read(&dat,1)  -  read(&dat,x)
 //3 Byte + Stop:  init (-) start(adr+1,0) (-) read(&dat,0)  -  I2Cx_read_last2(&dat1,&dat2)
 //
 //4 Byte + Stop:  init (-) start(adr+1,0) (-) read(&dat,0) (-) read(&dat,0)  -  read(&dat,1)  -  read(&dat,x)
 //4 Byte + Stop:  init (-) start(adr+1,0) (-) read(&dat,0) (-) read(&dat,0)  -  I2Cx_read_last2(&dat1,&dat2)
 //
 //5 Byte + Stop:  init (-) start(adr+1,0) (-) read(&dat,0) (-) read(&dat,0) (-) read(&dat,0)  -  read(&dat,1)  -  read(&dat,x)
 //...
 //n Byte + Stop:  init (-) I2Cx_read_n(adr+x,daten[],anz)
 //
 //Master Transmitter:
 //*******************
 //
 //1 Byte + Stop:  init (-) start(adr+0,x) (-) send(data,1)
 //2 Byte + Stop:  init (-) start(adr+0,x) (-) send(data,0) (-) send(data,1)
 //3 Byte + Stop:  init (-) start(adr+0,x) (-) send(data,0) (-) send(data,0) (-) send(data,1)
 //...
 //n Byte + Stop (Pause zw. Byteübertragung mögl.): init (-) I2Cx_send_n(adr+x,daten[],anz,pause)
 //
 //Legende:
 //********
 //
 // "(-)":  Sofort danach oder nach einer beliebig langen Pause folgt die nächste Operation (Achtung: bei einem Reset kann es zu Error Nr. 0x86 kommen - Die Busleitungen sind noch immer auf 0, da mitten im Datenverkehr abgebrochen wurde)
 //  " - ":  Sofort danach folgt die nächste Operation (Es muss sofort reagiert werden)
 //

//Fehlercodes:
//************
//
// Ein Fehlercode beinhaltet zwei Arten von Daten: Wo der Fehler auftrat und was der Fehlergrund ist.
// Die ersten 4bit (MSB) geben an, in welcher Routine sich der Fehler ereignet hat (Fehlerposition), die
// letzten 4bit (LSB) geben die Fehlerart an. Ist kein Fehler aufgetreten, wird 0x00 als Code verwendet,
// die Fehlerposition nicht miteinberechnet.
// Fehlercode-Berechnung:  "Fehlercode" = "Fehlerposition" | "Fehlerart";
//
#define info_success         0x00  //Kein Fehler aufgetreten (immer 0x00)
/*Fehlerposition:*/
#define err_at_init       0x80   //Fehler in der Initialisierungsroutine
#define err_at_start         0x40   //Fehler in der Start- und Adressierungsroutine
#define err_at_send       0x20  //Fehler in der Senderoutine
#define err_at_read       0x10  //Fehler in der Empfangsroutine
/*Fehlerarten:*/
#define error_I2C_running    0x01  //Der I2C-Bus ist bereits in Betrieb und kann nicht mehr initialisiert werden
#define error_I2C_disabled   0x02  //Es kann keine Startbedingung erzeugt werden, da der Bus außer Betrieb ist (Keine Initialisierung)
#define error_no_master      0x03  //Der I2C-Bus befindet sich nicht im Mastermode ("start()": Peripherie-Fehler;  "send()": Startbedingung erzeugt?)
#define error_no_transmitter 0x04   //Es können keine Daten gesandt werden, da der Slavebaustein zum lesen Adressiert wurde (R/W-bit muss 0 sein)
#define error_not_addressed  0x05   //Die Hardware wurde nicht adressiert, NACK erhalten (Wurde die richtige Adresse eingestellt?)
#define error_timeout_busy   0x06  //Der Bus kann derzeit noch nicht initialisiert werden, da bereits ein Datenverkehr ausgeführt wird (Leitung auf "LOW" - PullUp-Widerstände vorhanden?)
#define error_timeout_start  0x07   //Bei der Startbedingung ist es zu einem Time-Out gekommen (Ist der Bus belegt (Multimaster-Mode)?, Ist vielleicht noch eine Datenübertragung im gange?)
#define error_timeout_stop   0x08  //Bei der Stopbedingung ist es zu einem Time-Out gekommen (Ist vielleicht noch eine Datenübertragung im gange?)
#define error_timeout_send   0x09  //Die Daten wurden nicht übertragen (Timeout) (Verwendet der Slave Clock-Streching? Sind die Busleitungen auf "LOW" gefallen?)
#define error_NACK       0x0A  //Der Slave hat auf die Daten mit einem NACK geantwortet (Der Fehler liegt auf der Slave-Seite oder es wurde dessen Übertragungsprotokoll verletzt/beendet)
#define error_no_data     0x0B  //Bei "read()". Entweder wurde noch kein Baustein adresiert oder nach Beendigung der letzten Übertragung sind keine ausstehenden Daten mehr vorhanden (Adressiert?  "I2Cx_data_available()" verwendet?)

typedef struct
{
  uint32_t I2C_ClockSpeed;          /*!< Specifies the clock frequency.
                                         This parameter must be set to a value lower than 400kHz */

  uint16_t I2C_Mode;                /*!< Specifies the I2C mode.
                                         This parameter can be a value of @ref I2C_mode */

  uint16_t I2C_DutyCycle;           /*!< Specifies the I2C fast mode duty cycle.
                                         This parameter can be a value of @ref I2C_duty_cycle_in_fast_mode */

  uint16_t I2C_OwnAddress1;         /*!< Specifies the first device own address.
                                         This parameter can be a 7-bit or 10-bit address. */

  uint16_t I2C_Ack;                 /*!< Enables or disables the acknowledgement.
                                         This parameter can be a value of @ref I2C_acknowledgement */

  uint16_t I2C_AcknowledgedAddress; /*!< Specifies if 7-bit or 10-bit address is acknowledged.
                                         This parameter can be a value of @ref I2C_acknowledged_address */
}I2C_InitTypeDef;


#ifdef __I2C_LIB_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/****************************************************************************/
/*                                                */
/****************************************************************************/ 
EXPORT char I2C_error_message(USART_TypeDef* UARTx,char errorcode);
EXPORT char I2Cx_init(I2C_TypeDef* I2Cx);
EXPORT void I2Cx_disable(I2C_TypeDef* I2Cx);
EXPORT char I2Cx_start(I2C_TypeDef* I2Cx,char adresse,char single);
EXPORT char I2Cx_get_mode(I2C_TypeDef* I2Cx);
EXPORT char I2Cx_data_available(I2C_TypeDef* I2Cx);
EXPORT char I2Cx_read(I2C_TypeDef* I2Cx,char *daten,char last);
EXPORT char I2Cx_read_1(I2C_TypeDef* I2Cx,char adresse, char *daten);
EXPORT char I2Cx_read_last2(I2C_TypeDef* I2Cx,char *daten1, char *daten2);
EXPORT char I2Cx_read_n(I2C_TypeDef* I2Cx,char adresse, char daten[],int anzahl);
EXPORT char I2Cx_send(I2C_TypeDef* I2Cx,char daten,char stop);
EXPORT char I2Cx_send_n(I2C_TypeDef* I2Cx,char adresse, char daten[],int anzahl,int pause);

#undef  EXPORT

#endif
