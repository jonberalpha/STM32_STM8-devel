// Filename MGRM.C
#include "std_stm32_f103rb.h"

#define USART_SR USART1_BASE   // Calc peripheral address UART
#define RI  *((volatile unsigned long *)(BITBAND_PERI(USART_SR,5)))  // RXNE
 char Schalter=0;    // Zustand der Schalter
  int i;
 char auswahl, ADC, temp;
    char buffer[20];

   void ton(void);

int main (void)
{   
    init_leds_switches();
    B0_init_out();
    //----------LCD Ausgabe----------
    uart_init(9600);  //RS232-Verbindung mit 9600 Baud initialisiern
    uart_clear();    //UART Fenster löschen
    uart_setpos(1,1);  //An den Anfang springen
    uart_put_string("STM-CORTEX xxx-M3\xd\xa");
       
   ton();

   lcd_init();
   
   lcd_clear();   // 1.Zeile 1  Position 0 
   lcd_put_char('t'); 
   

   lcd_set_cursor(1,0);  // 2. zeile Position 0
   temp = 0xFA;
   sprintf(&buffer[0], "W =%x", temp);
   lcd_put_string(&buffer[0]);

   lcd_set_cursor(1,15);   // 2.Zeile  Position 15 
   lcd_put_char('X'); 


  
      do {
     //  if((USART1->SR & USART_SR_RXNE))
    if (RI)
       {  auswahl=uart_get_char();
         uart_put_char(auswahl+1); 
        } 
       //  DIL_LED_1=DIL_TASTER_1;  
    //  DIL_LED_3=DIL_TASTER_2;
     //DIL_LED_1=~DIL_LED_1;  
      DIL_LED_1=JOY_DOWN;
    //  Rxne=1;
       // wait_ms(100);
    // DIL_LED_3=~DIL_LED_3;
      DIL_LED_3=JOY_LEFT;
       // wait_ms(2);
       //B0_OUT=~B0_OUT;   
       // ton();
         wait_10us();
      Schalter = get_switches();
   
    //LED7=SW0;
     //ADC=adc1_convert(14);    //ADC-Wert von Kanal 14 ermitteln
       set_leds(Schalter);
    } while (1);
    
    }         
    
      
  void ton (void)
   {  
     
  do {     i++;
        wait_ms(2);
       B0_OUT=~B0_OUT;
        
  } while (i<=1000);      
    }
