/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   DC_Motor.c                                                    */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        27.05.2018 (last change)                                      */
/* Description: Ansteuerung und Drehzahlbestimmung eines DC-Motors mit Uhr    */
/******************************************************************************/
/* History:     V1.00  creation                                               */
/******************************************************************************/
#include "armv10_std.h"
#include "ADC_STM32F10X.h"

/*-------------------------- Function Prototypes -----------------------------*/
void init_ports(void);  //initialisiert uC-Ports
void TIM1_Config(void); //Timer 1 config, fuer Uhr
void TIM3_Config(void); //Timer 3 config, fuer Motoransteuerung
void TIM4_Config(void); //Timer 4 config, fuer Input Capture
void NVIC_init(char position, char priority); //NVIC initialisieren

/* ------------------------------- Defines -----------------------------------*/
#define TIM_IT_Update                ((uint16_t)0x0001) //TIM_interrupt_source
#define TIM_IT_CC4                   ((uint16_t)0x0010) //TIM_interrupt_source

/*---------------------------- Global Variables ------------------------------*/
double IC4ReadValue1 = 0,IC4ReadValue2=0; //Input Capture Variablen
int CaptureNumber = 0; //Unterscheidung 1. oder 2. Input Capture
int Capture = 0; //Capture Computation
int help = 0; //help fuer LCD-Update

/******************************************************************************/
/*           Interrupt Service Routine  Timer 1 (General Purpose Timer)       */
/******************************************************************************/
void TIM1_UP_IRQHandler(void)  //Timer 1, loest alle 1ms aus
{
  TIM1->SR &= ~0x01; //Interrupt pending bit loeschen (Verhinderung eines erneuten Interrupts)
  help = ~help;        //Hilfsvariable fuer das LCD-Displayupdate (alle 100ms)
}

/******************************************************************************/
/*           Interrupt Service Routine  Timer 4 (General Purpose Timer)       */
/******************************************************************************/
void TIM4_IRQHandler(void)
{
  if(((TIM4->SR & TIM_IT_Update) != 0) && ((TIM4->DIER&TIM_IT_Update) != 0)) //Update Interrupt Pending ?
  {
    TIM4->SR &= ~TIM_IT_Update; //Clear TIM4 Update Interrupt pending bit
  }
  if(((TIM4->SR & TIM_IT_CC4) != 0) && ((TIM4->DIER & TIM_IT_CC4) != 0)) //Capture Interrupt von Channel 4?
  {
    TIM4->SR &= ~TIM_IT_CC4; //Clear TIM4 Capture Interrupt pending bit for Channel 4
    if(CaptureNumber == 0)
    {
      IC4ReadValue1=TIM4->CCR4; //Get the Input Capture value for Channel 4
      CaptureNumber=1;
    }
    else if(CaptureNumber == 1)
    {
      IC4ReadValue2=TIM4->CCR4; //Get the Input Capture value for Channel 4
      if(IC4ReadValue2 > IC4ReadValue1) //Capture computation
      {
        Capture=(IC4ReadValue2-IC4ReadValue1);
      }
      else
      {
        Capture=((TIM4->ARR-IC4ReadValue1)+IC4ReadValue2); 
      }
      CaptureNumber=0;
    }
  }
}

/******************************************************************************/
/*                Initialization Timer 1 (General Purpose Timer)               */
/* Funktion:                                                                  */
/*   initialisiert Timer 1, der fuer die Uhr zustaendig ist ->INT=alle 100ms  */
/******************************************************************************/
void TIM1_Config(void)
{  
  /*-------------------------- Timer 1 konfigurieren -------------------------*/
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;  //Timer 1 Clock enable
  TIM1->SMCR &= ~0x07; //Timer 1 Clock Selection: CK_INT wird verwendet
  TIM1->CR1 &= 0x00; //CMS=00: Edge-aligned mode. DIR=0 --> Upcounter
  
  TIM1->PSC = 149; //Prescaler
  TIM1->ARR = 48000; //Auto-Reload Wert = Maximaler Zaehlerstand des Upcounters
  
  /*--------------------- Interrupt Timer 1 konfigurieren --------------------*/
  TIM1->DIER |= 0x01; //enable Interrupt bei einem UEV (UEberlauf/Unterlauf)
  NVIC_init(TIM1_UP_IRQn,2); //Enable Timer 1 Update Interrupt, Priority 2
  
  /*---------------------------- Enable Timer 1 ------------------------------*/
  TIM1->CR1 |= 0x01; //Enable Timer 1
}

/******************************************************************************/
/*                Initialization Timer 3 (General Purpose Timer)               */
/* Funktion:                                                                  */
/*   initialisiert Timer 3, der fuer die Motoransteuerung zustaendig ist      */
/*   CC4 CH4(PB1) als Output Compare verwendet ->INT=alle 100ms                */
/******************************************************************************/
void TIM3_Config(void)
{  
  /*------------------------- Timer 3 konfigurieren --------------------------*/
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  //Timer 3 Clock enable
  TIM3->SMCR &= ~0x07; //Timer 3 Clock Selection: CLK_INT wird verwendet
  TIM3->CR1  |= 0x010; // Auswahl des Timer Modus: Downcounter -> Zaehlt: von ARR bis 0
  
  TIM3->PSC = 100; //Prescaler
  TIM3->ARR = 40000; //Auto-Reload Wert = Maximaler Zaehlerstand des Downcounters
  
  /*--------------------- Interrupt Timer 3 konfigurieren --------------------*/
  TIM3->DIER = 0x01; //enable Interrupt bei einem UEV (UEberlauf / Unterlauf)
  
  /*------------- Configure Channel 4(PB1) as Output Compare -----------------*/
  TIM3->CCR4 = 20000; //1/4 von ARR-Wert=13200 als Level festgelegt
  TIM3->CCMR2 |= 0x7800; //CC4S=00, OC4PE=1, OC4M=111
  TIM3->CCER |= 0x01000; //Enable CH4 als Output Compare
  
  /*---------------------------- Enable Timer 3 ------------------------------*/
  TIM3->CR1 |= 0x01; //Enable Timer 3
}

/******************************************************************************/
/*                Initialization Timer 4 (General Purpose Timer)               */
/* Funktion:                                                                  */
/*   initialisiert Timer 4, der fuer die Drezahl zustaendig ist                */
/*   CC4 CH4(PB9) als Input Capture verwendet ->INT=alle 10ms                 */
/******************************************************************************/
void TIM4_Config(void)
{
  /*-------------------- Configure Timer 4 CR1 - Register --------------------*/
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //Timer 4 Clock enable
  TIM4->SMCR &= ~0x07; //Timer 4 Clock Selection: CLK_INT wird verwendet
  TIM4->CR1 &= 0x00; //CMS=00: Edge-aligned mode. DIR=0 --> Upcounter
  
  TIM4->PSC = 710; //Prescaler
  TIM4->ARR = 10000; //Auto-Reload Value

  /*---------------- Configure Channel 4(PB9) as Input Capture ---------------*/
  TIM4->CCMR2 |= 0x0100; //IC4, no filter
  TIM4->CCER |= 0x01000;
  
  /*--------------------- Enable Interrupts for Timer 4 ----------------------*/
  TIM4->DIER  |= 0x011; //Enable Update Interrupt, Enable the CC4 Interrupt Request
  NVIC_init(TIM4_IRQn,1); //Enable Timer 4 Interrupt, Priority 1
  
  /*---------------------------- Enable Timer 4 ------------------------------*/
  TIM4->CR1 |= 0x01; //Enable Timer 4
}

/******************************************************************************/
/*                   NVIC_init(char position, char priority)                   */ 
/* Funktion:                                                                  */    
/*   UEbernimmt die vollstaendige Initialisierung eines Interrupts im Nested  */
/*   Vectored Interrupt Controller (Prioritaet setzen, Ausloesen verhindern,  */
/*   Interrupt enablen)                                                       */
/* UEbergabeparameter: "position" = 0-67 (Nummer des Interrupts)              */
/*                     "priority" : 0-15 (Prioritaet des Interrupts)           */
/******************************************************************************/
void NVIC_init(char position, char priority) 
{  
  NVIC->IP[position]=(priority<<4);  //Setzen der Interrupt Prioritaet
  NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F)); //Verhindert, dass der Interrupt ausloest sobald er enabled wird 
  NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F)); //Enable Interrupt
} 

/******************************************************************************/
/*                   init_ports()                                              */ 
/* Funktion:                                                                  */    
/*   initialisiert uC-Ports  (PB9 as IC, PB1 as OC)                            */
/******************************************************************************/
void init_ports(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //GPIOB Clock enable
  
  GPIOB->CRH &= ~0x0F0; //PB9 Konfigurationsbit loeschen
  GPIOB->CRH |= 0x080;  //PB9 auf IPPU fuer Input Capture: Drehzahl
  GPIOB->ODR |= 0x0200; //Pull-Up fuer PB9 aktivieren
  
  GPIOB->CRL &= ~0x0F0; //PB1 Konfigurationsbit loeschen
  GPIOB->CRL |= 0x0B0;  //PB1 auf AFOPP fuer Output Compare: Motorantrieb
}

/******************************************************************************/
/*                              MAIN function                                 */
/******************************************************************************/
int main(void) 
{
  set_clock_72MHz(); //SYS_CLK auf HSE 72MHz einstellen
  
  volatile double um=0; //Drezahl
  
  int ADC = 1000; //Wert des DIL-Poti fuer PWM-Einstellung
  
  //lcd_init(); //LCD Anzeige initialisieren
  init_ports(); //initialisiert uC-Ports
  TIM1_Config(); //Timer 1 config, fuer Uhr
  TIM3_Config(); //Timer 3 config, fuer Motoransteuerung(Output Compare)
  TIM4_Config(); //Timer 4 config, fuer Drehzahl(Input Capture)
  
  while(1)
  {
    /*---------------- Motor PWM-Einstellung mit DIL-Poti --------------------*/
    ADC = adc_convert(7); //Wert von 0-4000; Channel 7 = PA7
    ADC = ADC * 10; //Wert*10
    if(ADC <= 1000) //PWM von 100%->High und 0%->Low verhindern
    {
      ADC = 1000;
    }
    if(ADC >= 33800) //PWM von 0%->High und 100%->Low verhindern
    {
      ADC = 33800;
    }
    TIM3->CCR4 = ADC; //ADC-Wert als Output Compare Level verwenden
    
    /*--------------------- Drehzahl-Anzeige --------------------------------*/
    um = Capture; //Umdrehungen pro min berechnen
  }
}
