// Messgeraet fuer CVI Kommunikation   gn_MGRM.c

// Includes
#include "std_stm32_f103rb.h"  // mit Definitionen aus <stm32f10x.h>

// Defines
#define RI *((volatile unsigned long *)(BITBAND_PERI(USART1_BASE,5)))  // RXNE

// Global Variables
char auswahl;
int ADC;
char buffer[20];

// Prototypes
void ton(void);

int main(void)
{    
  init_leds_switches();
  B0_init_out();         //Lautsprecher aktivieren
  ton();
  lcd_init();        //LCD initialisieren
  lcd_clear();         //1.Zeile 1  Position 0
  lcd_put_char('-');
  lcd_set_cursor(0,2);    //1. zeile Position 3
  sprintf(&buffer[0], "RM-Messger\xe1t");
  lcd_put_string(&buffer[0]);

  uart_init(640800);  //RS232-Verbindung mit 640800 Baud initialisiern
  uart_clear();    //UART Fenster löschen   <ESC>[2J
  uart_put_string("STM-CORTEX RM-M3\xd\xa");

  do 
  {  
    ADC = adc1_convert(14);    //12-bit-ADC-Wert von Kanal 14 ermitteln
    lcd_set_cursor(1,1);     //2.Zeile  Position 6
    //sprintf(&buffer[0], "W = %4d",ADC);
    //lcd_put_string(&buffer[0]);

    if(RI)               //will der UART was ?
    {  
      auswahl=uart_get_char();  //Zeichen einlesen  von UART
      auswahl = auswahl | 0x20; //immer Kleinbuchstaben
      lcd_put_char(auswahl);
      switch(auswahl)           //Befehlsauswahl
      {
        case 'i':
          uart_put_string("(c) 2010: R.M._Temperaturmessgeraet v1.0 CM3 \n\r");
          break;
        case 'w':      // Ausgabe aktueller Temperaturwert auf UART
          sprintf(&buffer[0], "%04d\n\r", ADC); //  CR,LF
          uart_put_string(&buffer[0]);
          //printf("Wert = [%04d]\n", ADC);     // stürzt ab !!!!!
          break;
        default:
          uart_put_char(auswahl+1);      //V24 Modul Test auf Kurzschluss
          ton();                      //sonst: unbekannter Befehl !
      }
    }
  }
  while(1);
}

void ton (void)
{
  int i;
  for(i=1; i<=100; i++)  // 0.1 Sekunde lang
  { 
    wait_ms(1);        // 1kHz Rechteck
    B0_OUT = ~B0_OUT; // Piezo bit invertieren
  }
}
