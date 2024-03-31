/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   CNY70_LED_Test.c                                              */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        10.03.2018                                                    */
/* Description: LED mit digitalisierten Sensor ansteuern                      */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/

/*-----------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include "RTE_Components.h"
#include "rtx_os.h"                  // include rtx_os.h for types of RTX objects
#include "cmsis_os2.h"
#include "armv10_std.h"             // HTL Hollabrunn::HTL Hollabrunn:Libraries:STDLib
#include "timer.h"                  // inlude Timer to wait without blocked loop
#include  CMSIS_device_header

#define TIME 800 //in ms

/*------------------------------------------------------------------------------
 * Static Variables
 *----------------------------------------------------------------------------*/
static volatile char sw;       //Schalterstellung

/*------------------------------------------------------------------------------
 * Thread ID
 *----------------------------------------------------------------------------*/
osThreadId_t th1;
osThreadId_t th2;
osThreadId_t th3;

/*-----------------------------------------------------------------------------
 * Semaphore erzeugen

osSemaphoreId_t osSemaphoreNew  (max_count, initial_count, attr);  

max_count       =>  maximale Anzahl verfügbarer Token
initial_count   =>  Anfangsanzahl verfügbarer Token
attr            =>  Semaphor-Attribute; NULL: Standardwerte
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Semaphore ID
 *----------------------------------------------------------------------------*/
osSemaphoreId_t bisem;

/*-----------------------------------------------------------------------------
 * Thread 1 - Schalterstellung auf LEDs abbilden
 *----------------------------------------------------------------------------*/
__NO_RETURN void thread1 (void *argument)
{
  while(1)
  {
    osSemaphoreAcquire(bisem, osWaitForever);    //Semaphore sperren
    
    //Schalterstellung auf LEDs abbilden
    sw = ~get_switches(); //softwaremaeßiges Invertieren
    set_leds(sw);
    
    osSemaphoreRelease(bisem);      //Semaphore freigeben
  }
}

/*------------------------------------------------------------------------------
 * Thread 2 - Zeige aktuelle Schalterstellung am LCD-Display
 *----------------------------------------------------------------------------*/
__NO_RETURN void thread2 (void *argument)
{
  char sw_str[20];
  
  while(1)
  {
    osSemaphoreAcquire(bisem, osWaitForever);    //Semaphore sperren
      
    lcd_set_cursor(1,0);              //Curser auf richtige Position setzen (Zeile 2/Spalte 1)
    sprintf(sw_str, "SW = 0x%02hhx   ", sw);
    lcd_put_string(sw_str);           //Ausgabe der Hilfsausgabe für den Benutzer
    
    osSemaphoreRelease(bisem);      //Semaphore freigeben
  }
}

/*------------------------------------------------------------------------------
 * Thread 3 - Application Running: animiert Ladepunkte am LCD-Display
 *----------------------------------------------------------------------------*/
__NO_RETURN void thread3 (void *argument)
{
  int delay = 0;
  int i = 0;
  char loading_points[3][4] = {".  ",".. ","..."};
  
  while(1)
  {
    if(tick)
    {
      tick = 0;
      if(delay == TIME)
      {
        delay = 0;
        
        /***** Critical Section *****/
        osSemaphoreAcquire(bisem, osWaitForever);    //Semaphore sperren
        
        lcd_set_cursor(0,9);
        lcd_put_string(loading_points[i]);
        
        osSemaphoreRelease(bisem);      //Semaphore freigeben
        /****************************/
        
        if(i == 2)
        {
          i = 0;
        }
        else
        {
          i++;
        }
      }
      delay++;
    }
  }
}

/*------------------------------------------------------------------------------
 * Hauptprogramm
 *----------------------------------------------------------------------------*/
int main (void)
{
  init_leds_switches();
  
  TIM1_Config();                    //Config TIM1
  
  lcd_init();                        //LCD initialisieren  
  lcd_clear();                      //Inhalt des LCD-Displays loeschen
  lcd_set_cursor(0,0);
  lcd_put_string("RTOS TEST      ");
  
  SystemCoreClockUpdate();          //Takt einstellen
  
  osKernelInitialize();             //Initialisieren CMSIS-RTOS
  //:CODE://
  th1 = osThreadNew(thread1, NULL, NULL); //Thread1 erstellen
  th2 = osThreadNew(thread2, NULL, NULL); //Thread2 erstellen
  th3 = osThreadNew(thread3, NULL, NULL); //Thread2 erstellen
  if(!th1 || !th2)
  {
    //Error handling
    lcd_set_cursor(1,0);
    lcd_put_string("No Thread       ");
  }
  
  bisem = osSemaphoreNew(1, 1, NULL);      //bisem Semaphore erstellen
  if (!bisem)
  {
    //Error handling
    lcd_set_cursor(1,0);                //Curser auf richtige Position setzen (Zeile 2/Spalte 1)
    lcd_put_string("No Semaphore    ");  //Ausgabe der Hilfsausgabe für den Benutzer
  }
  
  osKernelStart();                  //Threads ausfuehren
  
  while(1);
}
