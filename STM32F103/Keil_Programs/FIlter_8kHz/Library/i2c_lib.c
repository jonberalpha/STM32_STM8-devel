#define __I2C_LIB_MOD
#include "i2c_lib.h"
#include "gpio_lib.h"
#include "uart_lib.h"

/********************************************************************************************************************************/
/* \ *\     I 2 C - L I B R A R Y                                          23.07.2010 \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    HTL Hollabrunn   AUSTRIA                                                  \* / */
/* / *\    Author:         Maier Jakob                                               \* \ */
/* \ *\    Date:         März 2011                                             \* / */
/* / *\    Version:       2.0                                                 \* \ */
/* \ *\    Description:  Beinhaltet alle wesentlichen Unterprogramme die zur Kommunikation über den I2C-Bus          \* / */
/* / *\            als Master notwendig sind                                     \* \ */
/* \ *\    (c) Maier Jakob                                                   \* / */
/********************************************************************************************************************************/

//Versionshinweise:
//*****************
//
//Ver. 1.0:  Funktionen für I2C1
//Ver. 2.0:  Erweitert für I2C1 und I2C2, in I2C_error_message kann jeder UART gewählt werden



/********************************************************************************************************************************/
/* \ *\     U N T E R P R O G R A M M:    I2C_error_message(char errorcode)                           \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    Funktion:       Gibt eine Fehleranalyse am Uart (1) aus. Der Fehler wird weitergegeben um das UPro  direkt in  \* / */
/* / *\             einer Fehlerabfrage verwenden zu können:  "if(I2C_error_message(I2Cx_init())!=info_success)"  \* \ */
/* \ *\    Fehlerposition:   -                                                    \* / */
/* / *\    Übergabeparameter: Fehlercode                                             \* \ */
/* \ *\    Rückgabewert:       Fehlercode                                                \* / */
/* / *\    Nächster Schritt:  -                                                 \* \ */
/********************************************************************************************************************************/
char I2C_error_message(USART_TypeDef* UARTx,char errorcode)
{
  if(errorcode==info_success)            //Kein Fehler
  {  return info_success;
  }
  UARTx_printf(UARTx,"%s\n\r=== I2C-Error ===\n\r*****************\n\r\n\r",TXT_RED);
  UARTx_printf(UARTx,"   Fehlerposition:       <0x%02x>\n\r   Fehlerart:            <0x%02x>\n\r\n",(errorcode&0xF0),(errorcode&0x0F));

    UARTx_printf(UARTx,"Analyse:\n\r========\n\r   Der Fehler ist ");
  switch(errorcode&0xF0)
  {  case err_at_init:      UARTx_put_string(UARTx,"in der Initialisierung");              break;
    case err_at_start:    UARTx_put_string(UARTx,"bei der Startbedingung oder der Adressierung");    break;
    case err_at_send:    UARTx_put_string(UARTx,"beim Senden");                    break;
    case err_at_read:    UARTx_put_string(UARTx,"beim Lesen");
  }
  UARTx_put_string(UARTx," aufgetreten.\n\r   Fehler-Beschreibung:    \"");
  switch(errorcode&0x0F)
  {  case error_I2C_running:      UARTx_put_string(UARTx,"error_I2C_running");            break;
    case error_I2C_disabled:    UARTx_put_string(UARTx,"error_I2C_disabled");            break;
    case error_no_master:      UARTx_put_string(UARTx,"error_no_master");              break;
    case error_no_transmitter:    UARTx_put_string(UARTx,"error_no_transmitter");            break;
    case error_not_addressed:    UARTx_put_string(UARTx,"error_not_addressed");            break;
    case error_timeout_busy:    UARTx_put_string(UARTx,"error_timeout_busy");            break;
    case error_timeout_start:    UARTx_put_string(UARTx,"error_timeout_start");            break;
    case error_timeout_stop:    UARTx_put_string(UARTx,"error_timeout_stop");            break;
    case error_timeout_send:    UARTx_put_string(UARTx,"error_timeout_send");            break;
    case error_NACK:        UARTx_put_string(UARTx,"error_NACK");                break;
    case error_no_data:        UARTx_put_string(UARTx,"error_no_data");
  }
  UARTx_put_string(UARTx,"\".\n\r\n\r");
  return errorcode;
}


/********************************************************************************************************************************/
/* \ *\     U N T E R P R O G R A M M:    I2Cx_init(I2C_TypeDef* I2Cx)                             \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    Funktion:       Initialisiert den I2C-Bus im Standard-Modus (100kHz). Port-Leitungen:   kein remap            \* / */
/* / *\             Es wird die GPIO und I2C-Peripherie enabled. I2C kann mit "I2Cx_disable()" disabled werden    \* \ */
/* \ *\    Fehlerposition:   0x80                                                  \* / */
/* / *\    Übergabeparameter: -                                                 \* \ */
/* \ *\    Rückgabewert:     Fehlercode                                                \* / */
/* / *\    Nächster Schritt:  "I2Cx_start()"                                           \* \ */
/********************************************************************************************************************************/
char I2Cx_init(I2C_TypeDef* I2Cx)
{
  char counter;
  /*Config:  SCL=PB6, SDA=PB7, Standard-Mode, 100kHz*/

  if(I2Cx==I2C1)
  {  RCC->APB1ENR  |=  RCC_APB1ENR_I2C1EN;          // Clock enable I2C1  (I2C1EN - Flag)
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;          // Peripheral Reset Register (I2C1RST=0)
    AFIO->MAPR    &= ~AFIO_MAPR_I2C1_REMAP;          // I2C1_REMAP - Flag  (Remap auf 0 setzen --> PB6=SCL; PB7=SDA)
    init_port(GPIOB, 6, 7,GPO_pushpull,frequency_2MHz);    // PB6/PB7   = General Purpose Output   (CNF1 = 0  CNF0 = Mode1 = Mode0 = 1   ODR: 0 or 1)
    OUT_PB6=1;
    OUT_PB7=1;
    init_port(GPIOB, 6, 7,AFO_opendrain,frequency_50MHz);  // PB6/PB7   = Alternate Function Output Open Drain(connected with on-chip peripheral  CNF1 = CNF0 = Mode1 = Mode0 = 1   ODR: don't care)
  }else
  {  RCC->APB1ENR  |=  RCC_APB1ENR_I2C2EN;          // Clock enable I2C2  (I2C2EN - Flag)
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST;          // Peripheral Reset Register (I2C2RST=0)
    init_port(GPIOB,10,11,GPO_pushpull,frequency_2MHz);    // PB10/PB11 = General Purpose Output   (CNF1 = 0  CNF0 = Mode1 = Mode0 = 1   ODR: 0 or 1)
    OUT_PB10=1;
    OUT_PB11=1;
    init_port(GPIOB,10,11,AFO_opendrain,frequency_50MHz);  // PB10/PB11 = Alternate Function Output Open Drain(connected with on-chip peripheral  CNF1 = CNF0 = Mode1 = Mode0 = 1   ODR: don't care)
  }

  counter=0;
  while((I2Cx->SR2&I2C_SR2_BUSY)!=0)              // Busy - Flag;  (Es wird gerade auf dem Bus kommuniziert, kann auch ein anderer Master sein)
  {  counter++;                        // Dieses Flag wartet auf eine Stopcondition. Bei einem Hardware-Defekt würde das bit nicht mehr rückgesetzt werden.
    if(counter>=255)                    // Time-Out
    {  return (err_at_init|error_timeout_busy);      //[Error]    Time-Out. Die Bus ist belegt. Um eine Endlosschleife zu vermeiden, wird die Initialisierungsroutine verlassen.
    }
  }

  if((I2Cx->CR1&I2C_CR1_PE)!=0)                // PE - Flag; (Kontrollieren, ob I2C disabled ist, da einige Register sonst nicht verändert werden dürfen)
  {  return (err_at_init|error_I2C_running);          //[Error]    Der Bus kann nicht initialisiert werden da er bereits in Betrieb ist
  }

    I2Cx->CR2    = I2C_CR2_FREQ_3;                  // Interrupts diabled, Freq=8MHz (Interne Peripherie-Frequenz)
  I2Cx->CCR    = 0xC003;                    // Standard Mode (100kHz), für 8MHz Peripheriefrequenz berechnet
  I2Cx->TRISE  = 0x0002;                    // Maximal erlaubt Rise-Time, für 8MHz Peripheriefrequenz und Standard-Mode berechnet

  I2Cx->CR1   = I2C_CR1_PE;                    // I2C-Mode, Peripheral enable  (Die Start, Stop und PEC - bits müssen nicht kontrolliert werden (disabled))
  return info_success;
}


/********************************************************************************************************************************/
/* \ *\     U N T E R P R O G R A M M:    I2Cx_disable(I2C_TypeDef* I2Cx)                             \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    Funktion:       Schaltet die I2C-Peripherie ab. Es ist keine Buskommunikation mehr möglich. Die Ports werden  \* / */
/* / *\             als Output konfiguriert und auf 1 gesetzt.                             \* \ */
/* \ *\    Fehlerposition:   -                                                    \* / */
/* / *\    Übergabeparameter: -                                                 \* \ */
/* \ *\    Rückgabewert:     -                                                    \* / */
/* / *\    Nächster Schritt:  -                                                 \* \ */
/********************************************************************************************************************************/
void I2Cx_disable(I2C_TypeDef* I2Cx)              //Schaltet die I2C-Peripherie ab
{
  I2Cx->CR1     &= ~I2C_CR1_PE;                // PE - Flag;  (Erst nach Ende der Übertragung erlaubt, was durch die UPros sichergestellt wird)
  if(I2Cx==I2C1)
  {  RCC->APB1RSTR |=  RCC_APB1RSTR_I2C1RST;          // Peripheral Reset Register (I2C1RST=1)
    RCC->APB1ENR  &= ~RCC_APB1ENR_I2C1EN;          // Clock enable I2C1: 0  (I2C1EN - Flag)
    init_port(GPIOB, 6, 7,GPO_pushpull,frequency_2MHz);    // PB6/PB7   = General Purpose Output   (CNF1 = 0  CNF0 = Mode1 = Mode0 = 1   ODR: 0 or 1)
    OUT_PB6=1;
    OUT_PB7=1;
  }else
  {  RCC->APB1RSTR |=  RCC_APB1RSTR_I2C2RST;          // Peripheral Reset Register (I2C2RST=1)
    RCC->APB1ENR  &= ~RCC_APB1ENR_I2C2EN;          // Clock enable I2C2: 0  (I2C2EN - Flag)
    init_port(GPIOB,10,11,GPO_pushpull,frequency_2MHz);    // PB10/PB11 = General Purpose Output   (CNF1 = 0  CNF0 = Mode1 = Mode0 = 1   ODR: 0 or 1)
    OUT_PB10=1;
    OUT_PB11=1;
  }
  /*Achtung: Port-Konfiguration und Remap-Register bleiben bestehen!*/
}


/********************************************************************************************************************************/
/* \ *\     U N T E R P R O G R A M M:    I2C1_start(I2C_TypeDef* I2Cx,char adresse,char single)                 \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    Funktion:        Erzeugt eine Startbedingung und Adressiert einen Slavebaustein. Je nach R/W-Bit ist man      \* / */
/* / *\              Master Transmitter (0) oder Master Receiver(1)                         \* \ */
/* \ *\    Fehlerposition:    0x40                                                  \* / */
/* / *\    Übergabeparameter:  "adresse": 7bit Adresse und Read/!Write - Bit   "single": Wenn das R/W-Bit 1 ist und nur ein \* \ */
/* \ *\                       Byte empfangen werden soll ist "single" ungleich 0.                       \* / */
/* / *\    Rückgabewert:      Fehlercode                                               \* \ */
/* \ *\    Nächster Schritt:   Senden oder Empfangen                                       \* / */
/********************************************************************************************************************************/
char I2Cx_start(I2C_TypeDef* I2Cx,char adresse,char single)  // Erzeugt eine Startbedingung und Adressiert den Slave (Master Transmitter oder Receiver)
{                          // Receiver: um nur ein einzelnes Byte zu empfangen (NACK nach 1. Byte) muss "single" auf ungleich 0 gesetzt werden, sonst 0
  char counter;
  if((I2Cx->CR1&I2C_CR1_PE)==0)          // PE - Flag; (Kontrollieren, ob I2C enabled ist, da der Bus sonst nicht initialisiert wurde)
    return (err_at_start|error_I2C_disabled);  //[Error]    Es kann keine Startbedingung erzeugt werden, da der Bus nicht initialisiert wurde


  I2Cx->CR1 |= I2C_CR1_START;            // Start - Flag;   (Startbedingung erzeugen)

  counter=0;
  while((I2Cx->SR1&I2C_SR1_SB)==0)        // SB - Flag;  (1: Start-Condition generated; Kontrolle ob die Startbedingung erfolgreich war)
  {  counter++;
    if(counter>=255)              // Time-Out
    {  return (err_at_start|error_timeout_start);//[Error]    Time-Out. Die Startbedingung ist fehlgeschlagen
    }
  }                        //Info: Die Startbedingung wurde erzeugt

  if((I2Cx->SR2&I2C_SR2_MSL)==0)          // MSL - Flag;  (0: Slave, 1: Master; Kontrolle ob erfolgreich umgeschalten wurde)
    return (err_at_start|error_no_master);    //[Error]    Es wurde nicht in den Mastermodus umgeschalten


  I2Cx->DR  =  adresse;              // 7 bit Adresse und R/!W - bit

  counter=0;
  while((I2Cx->SR1&I2C_SR1_ADDR)==0)        // ADDR - Flag;  (1: Adress sent; Kontrolle ob die Adresse gesendet und ein ACK empfangen wurde)
  {  counter++;
    if(counter>=255)              // Time-Out
    {  return (err_at_start|error_not_addressed);//[Error]    Time-Out. Die Hardware konnte nicht adressiert werden
    }
  }
  counter = I2Cx->SR2;              // Read (Um Daten übertragen zu können, SR1 bereits gelesen)

  if((adresse&0x01)!=0&&single==0)        // Master Receiver
  {  I2Cx->CR1 |= I2C_CR1_ACK;          // ACK - Flag; ACK-enable (Darf nicht in der Initialisierungsroutine gesetzt werden)
  }else if(adresse&0x01)
  {  I2Cx->CR1 &=~I2C_CR1_ACK;          // ACK - Flag; ACK disable (Receiver: es wird nur noch ein Byte empfangen)
    I2Cx->CR1 |= I2C_CR1_STOP;          // Stop - Flag; (Stop-Bedingung erzeugen)
  }
  return info_success;
}


/********************************************************************************************************************************/
/* \ *\     U N T E R P R O G R A M M:    I2Cx_get_mode(I2C_TypeDef* I2Cx)                           \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    Funktion:       Übergibt den Status des I2C-Busses (Slave, Master Transmitter oder Master Receiver)       \* / */
/* / *\             Damit kann im Nachhinein festgestellt werden was das R/W-Bit war.                 \* \ */
/* \ *\    Fehlerposition:   -                                                    \* / */
/* / *\    Übergabeparameter: -                                                 \* \ */
/* \ *\    Rückgabewert:     0x00: Der Bus ist deaktiviert  0x01: Der Bus wurde als Slave adressiert                \* / */
/* / *\                      0x02: Master Transmitter    0x03: Master Receiver                       \* \ */
/* \ *\    Nächster Schritt:  -                                                 \* / */
/********************************************************************************************************************************/
//Noch nicht getestet:
char I2Cx_get_mode(I2C_TypeDef* I2Cx)
{  if((I2Cx->CR1&I2C_CR1_PE)==0)          // PE - Flag; ob der Bus enabled ist
    return 0x00;

  if((I2Cx->SR2&I2C_SR2_MSL)==0)          // MSL - Flag;  (0: Slave, 1: Master)
    return 0x01;                // Slave

  if((I2Cx->SR2&I2C_SR2_TRA)!=0)          // TRA - Flag;  (0: Master receiver, 1: Master transmitter)
  {  return 0x02;                // Master transmitter
  }else
  {  return 0x03;                // Master receiver
  }
}


/********************************************************************************************************************************/
/* \ *\     U N T E R P R O G R A M M:    I2Cx_data_available(I2C_TypeDef* I2Cx)                        \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    Funktion:       Überprüft ob im Master-Receiver Modus Daten die Empfangen wurden zum Abruf bereit stehen     \* / */
/* / *\             Damit kann im Unterprogramm "I2Cx_read()" eine Endlosschleife vermiden werden           \* \ */
/* \ *\    Fehlerposition:   -                                                    \* / */
/* / *\    Übergabeparameter: -                                                 \* \ */
/* \ *\    Rückgabewert:     0x00: Keine Daten vorhanden   0x01: Daten im Datenregister                     \* / */
/* / *\    Nächster Schritt:  -                                                 \* \ */
/********************************************************************************************************************************/
char I2Cx_data_available(I2C_TypeDef* I2Cx)      // Überprüft, ob im Master-Receiver Mode Daten des Slave-Bausteins verfügbar sind
{  if((I2Cx->SR1&I2C_SR1_RXNE)!=0)          // RxNE - Flag; Ob ein Zeichen empfangen wurde
  {  return 0x01;                // Daten vorhanden (Da eine Stopbedingung auch dann erzeugt werden kann, wenn noch nicht alle Daten  abgefragt
  }                        //    wurden und dadurch das MSL und das TRA-Register rückgesetzt werden, können diese nicht abgefragt werden
  return 0x00;                  // Keine Daten vorhanden
}


/********************************************************************************************************************************/
/* \ *\     U N T E R P R O G R A M M:    I2Cx_read(I2C_TypeDef* I2Cx,char *daten,char last)                           \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    Funktion:       Liest die Daten aus, die vom Slave gesendet wurden. Sind keine Daten vorhanden wird solange   \* / */
/* / *\             gewartet, bis welche empfangen wurden                               \* \ */
/* \ *\    Fehlerposition:   0x10                                                  \* / */
/* / *\    Übergabeparameter: "daten": Variable in die die Daten geschrieben werden. "last": Wenn dass das vorletzte Byte   \* \ */
/* \ *\                      ist dass empfangen werden soll, ist "last" ungleich 0                        \* / */
/* / *\    Rückgabewert:     Fehlercode                                                \* \ */
/* \ *\    Nächster Schritt:  Lesen oder nach Stop "I2Cx_data_available()" und dann "I2Cx_read()" bzw. Start oder Disable   \* / */
/********************************************************************************************************************************/
char I2Cx_read(I2C_TypeDef* I2Cx,char *daten,char last)      // Empfängt Daten vom Slave (Master Receiver)
{                          // Wenn nur noch ein Byte empfangen werden soll (Das ist das vorletzte), muss "last" ungleich 0 gesetzt werden
  if((I2Cx->SR2&I2C_SR2_MSL)==0&&(I2Cx->SR1&I2C_SR1_RXNE)==0)  // MSL - Flag;  (0: Slave), RxNE-Flag (Daten vorhanden?)
    return (err_at_read|error_no_data);      //[Error]    Es wurde keine Startbedingung erzeugt oder es sind keine Daten der letzten Übertragung mehr vorhanden

  while((I2Cx->SR1&I2C_SR1_RXNE)==0);        // RxNE - Flag; Ob ein Zeichen empfangen wurde (BTF-Flag darf nicht verwendet werden- heißt, das das SR auch schon voll ist)
                          // Endlosschleife (mit "I2Cx_data_available()" verhinderbar)
  *daten=I2Cx->DR;
  if(last!=0)                      // Es wird nach diesen Daten nur noch 1 Byte empfangen
  {  I2Cx->CR1 &=~I2C_CR1_ACK;          // ACK - Flag; ACK disable (Es wird nur noch ein Byte empfangen)
    I2Cx->CR1 |= I2C_CR1_STOP;          // Stop - Flag; (Stop-Bedingung erzeugen)
  }
  return info_success;
}


/********************************************************************************************************************************/
/* \ *\     U N T E R P R O G R A M M:    I2Cx_read_1(I2C_TypeDef* I2Cx,char adresse, char *daten)             \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    Funktion:       Erzeugt eine Startbedingung, Übernimmt die Adressierung, empfangt ein einzelnes Byte und      \* / */
/* / *\             erzeugt dann wieder eine Stopbedingung                               \* \ */
/* \ *\    Fehlerposition:   0x40 und 0x10                                              \* / */
/* / *\    Übergabeparameter: "adresse": Slaveadresse (Das R/W-Bit wird im UPro auf 1 gesetzt) "daten": Variable in die das \* \ */
/* \ *\                      empfangene Byte geschrieben wird                                   \* / */
/* \ *\    Rückgabewert:     Fehlercode                                                \* \ */
/* / *\    Nächster Schritt:  Start oder Disable                                         \* / */
/********************************************************************************************************************************/
char I2Cx_read_1(I2C_TypeDef* I2Cx,char adresse, char *daten)      // Erledigt Startbedingung, Adressierung, Datenempfang (1 Byte) und Stopbedingung
{
  char get;
  get=I2Cx_start(I2Cx,(adresse|0x01),1);      // Startbedingung und Adressierung (für 1 Byte)
  if(get!=info_success)
    return get;                  // Fehlernummer zurückgeben

  get=I2Cx_read(I2Cx,daten,1);          // Daten empfangen und Stopbedingung generieren
  return get;                    // Fehlernummer (wenn vorhanden) zurückgeben
}


/********************************************************************************************************************************/
/* \ *\     U N T E R P R O G R A M M:    I2Cx_read_last2(I2C_TypeDef* I2Cx,char *daten1, char *daten2)           \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    Funktion:       Empfängt 2 Bytes und erzeugt dann eine Stopbedingung. Kann bei einem datenempfang ab 2 Bytes  \* / */
/* / *\             immer verwendet werden                                       \* \ */
/* \ *\    Fehlerposition:   0x10                                                  \* / */
/* / *\    Übergabeparameter: "daten1": Variable für das vorletzte Empfangene Byte    "daten2": Das letzte Empfangsbyte     \* \ */
/* \ *\    Rückgabewert:     Fehlercode                                                \* / */
/* / *\    Nächster Schritt:  Start oder Disable                                         \* \ */
/********************************************************************************************************************************/
char I2Cx_read_last2(I2C_TypeDef* I2Cx,char *daten1, char *daten2)  // Empfängt noch 2xDaten und erzeugt dann eine Stopbedingung
{
  char get;
  get=I2Cx_read(I2Cx,daten1,1);           // Daten lesen
  if(get!=info_success)
    return get;                  // Fehlernummer zurückgeben
  get=I2Cx_read(I2Cx,daten2,1);           // Daten lesen
  return get;                    // Fehlernummer (wenn vorhanden) zurückgeben
}


/********************************************************************************************************************************/
/* \ *\     U N T E R P R O G R A M M:    I2Cx_read_n(I2C_TypeDef* I2Cx,char adresse, char daten[],int anzahl)                   \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    Funktion:       Liest eine bestimmte Anzahl an Daten ein. Übernimmt auch die Startbedingung, die Adressierung \* / */
/* / *\             und die Stopbedingung. Das R/W-Bit wird im UPro gesetzt.                     \* \ */
/* \ *\    Fehlerposition:   0x40 und 0x20                                              \* / */
/* / *\    Übergabeparameter: "adresse":  Adresse des Slave-Bausteins "daten[]": Array, in das die Empfangsdaten geschrieben \* \ */
/* \ *\               werden  "anzahl": Wie viele Bytes empfangen werden sollen (ab 1 Byte)               \* / */
/* / *\    Rückgabewert:     Fehlercode                                                \* \ */
/* \ *\    Nächster Schritt:  Start oder Disable                                                 \* / */
/********************************************************************************************************************************/
char I2Cx_read_n(I2C_TypeDef* I2Cx,char adresse, char daten[],int anzahl)// Liest Daten mit n-mal ein (Übernimmt auch Start, Stop und Adressierung)
{
  char get;                    // Für Rückgabewerte
  int i;                      // Für die Zählschleife

  get=I2Cx_start(I2Cx,(adresse|0x01),(anzahl==1)?1:0);  // Zum lesen adressieren
  if(get!=info_success)
  {  return get;                  // Fehlernummer zurückgeben
  }

  for(i=1;i<=anzahl;i++)
  {  get=I2Cx_read(I2Cx,&(daten[i-1]),((i>=anzahl-1)?1:0));//Daten empfangen
    if(get!=info_success)
      return get;                // Fehlernummer zurückgeben
  }
  return info_success;
}


/********************************************************************************************************************************/
/* \ *\     U N T E R P R O G R A M M:    I2Cx_send(I2C_TypeDef* I2Cx,char daten,char stop)                   \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    Funktion:       Sendet ein einzelnes Byte an den Slave und kann dann eine Stopbedingung erzeugen.         \* / */
/* / *\             Das R/W-Bit muss 0 (Master Transmitter) gewesen sein.                       \* \ */
/* \ *\    Fehlerposition:   0x20                                                  \* / */
/* / *\    Übergabeparameter: "daten": Die Sendedaten  "stop": Wenn dies Variable ungleich 0 ist, wird nach der Übertragung \* \ */
/* \ *\             eine Stopbedingung erzeugt                                     \* / */
/* / *\    Rückgabewert:     Fehlercode                                                \* \ */
/* \ *\    Nächster Schritt:  Senden, oder wenn "stop" ungleich 0: Start oder Disable                     \* / */
/********************************************************************************************************************************/
char I2Cx_send(I2C_TypeDef* I2Cx,char daten,char stop)  // "stop": Ob nach dem senden eine Stop-bedingung erzeugt werden soll
{
  char counter;
  if((I2Cx->SR2&I2C_SR2_MSL)==0)          // MSL - Flag;  (0: Slave, 1: Master; Kontrolle ob eine Startbedingung durchgeführt wurde)
  {  return (err_at_send|error_no_master);    //[Error]    Es wurde nicht in den Mastermodus umgeschalten
  }

  if((I2Cx->SR2&I2C_SR2_TRA)==0)          // TRA - Flag;  (0: Master receiver, 1: Master transmitter)
  {  return (err_at_send|error_no_transmitter);  //[Error]   Das Read-Write Bit der Adresse war 1 - Receiver
  }

  I2Cx->DR  =  daten;                // Daten ins Register schreiben - löst Übertragung aus

  counter=0;
  while((I2Cx->SR1&I2C_SR1_BTF)==0)        // BTF - Flag; Ob das Zeichen übertragen wurde
  {  counter++;
    if(I2Cx->SR1&I2C_SR1_AF!=0)          // AF- Flag; Ack-Fehler
      return (err_at_send|error_NACK);    //[Error]    NACK erhalten
    if(counter>=255)              // Time-Out
      return (err_at_send|error_timeout_send);//[Error]    Time-Out. Das Zeichen wurde nicht uebertragen
  }

    if(stop!=0)                    // Stopbedingung erzeugen
  {  I2Cx->CR1 |=  0x0200;            // Stop - Flag; (Stop-Bedingung erzeugen)
    counter=0;
    while((I2Cx->SR2&I2C_SR2_MSL)!=0)      // MSL - Flag; Ob in den Slave-Mode umgeschalten wurde
    {  counter++;
      if(counter>=255)            // Time-Out
        return (err_at_send|error_timeout_stop);//[Error]    Time-Out. Die Stopbedingung ist fehlgeschlagen
    }
  }
  return info_success;
}


/********************************************************************************************************************************/
/* \ *\     U N T E R P R O G R A M M:    I2Cx_send_n(I2C_TypeDef* I2Cx,char adresse, char daten[],int anzahl,int pause)           \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    Funktion:       Übernimmt die Startbedingung und die Adressierung. Sendet danach eine Reihe an Daten an den   \* / */
/* / *\             Slave wobei dazwischen Pausen eingelegt werden können. Danach kommt es zu einer Stopcondition \* \ */
/* \ *\    Fehlerposition:   0x40 und 0x20                                              \* / */
/* / *\    Übergabeparameter: "adresse": Slaveadresse (Das R/W-Bit wird im UPro auf 0 gesetzt)  "daten[]": Array dessen      \* \ */
/* \ *\            Inhalt an den Slave gesendet wird   "anzahl": Anzahl der Bytes aus dem Array die gesendet     \* / */
/* / *\                      werden  "pause": Pause in Millisekunden die zwischen den Übertragungen eingelegt wird       \* \ */
/* \ *\    Rückgabewert:     Fehlercode                                                \* / */
/* / *\    Nächster Schritt:  Start oder Disable                                         \* \ */
/********************************************************************************************************************************/
char I2Cx_send_n(I2C_TypeDef* I2Cx,char adresse, char daten[],int anzahl,int pause)// Sendet Daten n-mal mit einer Pause (Übernimmt auch Start, Stop und Adressierung, pause in ms)
{
  char get;
  int i;                      // Für die Zählschleife
  get=I2Cx_start(I2Cx,(adresse&0xFE),0);      // Zum schreiben adressieren
  if(get!=info_success)
  {  return get;                  // Fehlernummer zurückgeben
  }
  for(i=1;i<=anzahl;i++)
  {  get=I2Cx_send(I2Cx,daten[i-1],((i==anzahl)?1:0));//Daten senden
    if(get!=info_success)
      return get;                // Fehlernummer zurückgeben
    wait_ms(pause);                // Pause
  }
  return info_success;
}


/* Wird nicht benötigt, da die Stopbedingung bereits in den Sende- und Empfangroutinen integriert ist. */
/********************************************************************************************************************************/
/* \ *\     U N T E R P R O G R A M M:    ()                                              \* / */
/* / *\--------------------------------------------------------------------------------------------------------------------\* \ */
/* \ *\    Funktion:                                                         \* / */
/* / *\                                                             \* \ */
/* \ *\    Fehlerposition:                                                      \* / */
/* / *\    Übergabeparameter:                                                 \* \ */
/* \ *\    Rückgabewert:                                                        \* / */
/* / *\    Nächster Schritt:                                                  \* \ */
/********************************************************************************************************************************/
/*    char I2C1_stop()
    {
      char counter;
      if((I2C1->CR1&0x0001)==0)            // PE - Flag; (Kontrollieren, ob I2C1 enabled ist, da der Bus sonst nicht initialisiert wurde)
      {  return error_I2C1_disabled;          //[Error]    Es kann keine Stopbedingung erzeugt werden, da der Bus nicht initialisiert wurde
      }
      I2C1->CR1 |=  0x0200;              // Stop - Flag; (Stop-Bedingung erzeugen)

      counter=0;
      while((I2C1->SR2&0x0001)!=0)          // MSL - Flag; Ob in den Slave-Mode umgeschalten wurde
      {  counter++;
        if(counter>=255)              // Time-Out
        {  return error_timeout_stop;        //[Error]    Time-Out. Die Stopbedingung ist fehlgeschlagen
        }
      }
      return info_success;
    }
*/

