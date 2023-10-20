/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2018 All rights reserved. AUSTRIA   */ 
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
 *---------------------------------------------------------------------------*/
#define STM32F103x6
#include <RTE_Components.h>
#include <rtx_os.h>                  // include rtx_os.h for types of RTX objects
#include <cmsis_os2.h>
#include <stm32f1xx.h>

//#define CMSIS_device_header

/*----------------------------------------------------------------------------
 * Defines
 *---------------------------------------------------------------------------*/
#define GPIOC_ODR GPIOC_BASE + 3*sizeof(uint32_t)
#define GPIOB_ODR GPIOB_BASE + 3*sizeof(uint32_t)

#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))

#define LED1  *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,0)))  // PB0
#define LED2  *((volatile unsigned long *)(BITBAND_PERI(GPIOC_ODR,13)))  // PC13

#define TIME 1000 //in ms

/*----------------------------------------------------------------------------
 * Prototypes
 *---------------------------------------------------------------------------*/
static void GPIO_init(void);

/*----------------------------------------------------------------------------
 * Static Variables
 *---------------------------------------------------------------------------*/
static volatile char sw;       //Schalterstellung

/*----------------------------------------------------------------------------
 * Thread ID
 *---------------------------------------------------------------------------*/
osThreadId_t th1;
osThreadId_t th2;

/*----------------------------------------------------------------------------
 * Semaphore erzeugen

osSemaphoreId_t osSemaphoreNew  (max_count, initial_count, attr);  

max_count       =>  maximale Anzahl verfügbarer Token
initial_count   =>  Anfangsanzahl verfügbarer Token
attr            =>  Semaphor-Attribute; NULL: Standardwerte
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Semaphore ID
 *---------------------------------------------------------------------------*/
//osSemaphoreId_t bisem;

/*----------------------------------------------------------------------------
 * Subroutines
 *---------------------------------------------------------------------------*/
static void GPIO_init(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  
  GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
  GPIOC->CRH |= (GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0);
  
  GPIOB->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
  GPIOB->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);
}

/*----------------------------------------------------------------------------
 * Thread 1 - Schalterstellung auf LEDs abbilden
 *---------------------------------------------------------------------------*/
__NO_RETURN void thread1 (void *argument)
{
  while(1)
  {
    //osSemaphoreAcquire(bisem, osWaitForever);    //Semaphore sperren
    
    LED1 = ~LED1;
    osDelay(1000);
    //osSemaphoreRelease(bisem);      //Semaphore freigeben
  }
}

/*----------------------------------------------------------------------------
 * Thread 2 - Zeige aktuelle Schalterstellung am LCD-Display
 *---------------------------------------------------------------------------*/
__NO_RETURN void thread2 (void *argument)
{
  while(1)
  {
    LED2 = ~LED2;
    osDelay(1000);
  }
}

/*----------------------------------------------------------------------------
 * Hauptprogramm
 *---------------------------------------------------------------------------*/
int main (void)
{
  SystemCoreClockUpdate();
  osKernelInitialize();             //Initialisieren CMSIS-RTOS

  GPIO_init();

  th1 = osThreadNew(thread1, NULL, NULL); //Thread1 erstellen
  th2 = osThreadNew(thread2, NULL, NULL); //Thread2 erstellen

  //Error handling
  if(!th1 || !th2)
  {
    // e.g. UART
  }
  
  //bisem = osSemaphoreNew(1, 1, NULL);      //bisem Semaphore erstellen
  //if (!bisem)
  //{
  //  //Error handling
  //  lcd_set_cursor(1,0);                //Curser auf richtige Position setzen (Zeile 2/Spalte 1)
  //  lcd_put_string("No Semaphore    ");  //Ausgabe der Hilfsausgabe für den Benutzer
  //}
  
  osKernelStart();                  //Threads ausfuehren
  
  while(1);
}
