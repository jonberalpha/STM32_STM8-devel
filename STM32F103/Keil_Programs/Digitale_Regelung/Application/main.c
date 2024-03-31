/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   Main.c                                                        */
/* Autor:       Josef Reisinger, Gerald Zottl                                 */
/* Version:     V1.00                                                         */
/* Date:        07/02/2016                                                    */
/* Description: Demoprogramm Temperaturregelung mit Digitalem Regler          */
/******************************************************************************/
/* History:     V1.00  creation                                               */
/*              V1.10                                                         */
/******************************************************************************/
/*  Visualization:                                                            */               
/*  PA9     -> UART1_TX (SvVis3 System Visualization)                         */
/*  PA10    -> UART1_RX (SvVis3 System Visualization)                         */
/*                                                                            */
/*  Sensors:                                                                  */
/*  PB1     -> ADC1_IN9  Analog Input for desired Value (Sollwert)            */
/*  PC4     -> ADC1_IN14 Analog Input for measured Value (Istwert)            */
/*                                                                            */
/*  PWM - Signals:                                                            */
/*  PB6 -> PWM Signal Channel 1 of Timer 4                                    */
/*  PB7 -> PWM Signal Channel 2 of Timer 4                                    */
/*  PB8 -> PWM Signal Channel 4 of Timer 4                                    */
/*  PB9 -> PWM Signal Channel 4 of Timer 4                                    */
/*                                                                            */
/******************************************************************************/

//=============================================================================
// control engineering library
//=============================================================================

#define PTn_NO_OF_TIMECONSTS 4

typedef struct PTn_AttributesType
{
  float Ta;   // sample period
  float Kp;   // proportial amplification
  float Tn[PTn_NO_OF_TIMECONSTS];

  //---private: shouldn't be modified; read-only---
  float TnValue[PTn_NO_OF_TIMECONSTS];
}PTn_AttributesType;


typedef struct PID_AttributesType
{
  float Ta;       // sample period
  float Kp;       // proportial amplification
  float Ki;       // integrator
  int AntiWindup; // enable AntiWindup (bool)
  float Kd;       // differentiator
  float DT1;      // time const of differentiator

  //---private: shouldn't be modified; read-only---
  float PValue;
  float IValue;
  float DValue;
  float DT1Value;
  float Output;
}PID_AttributesType;


/*static*/ /*inline*/ float limit_value(float Value, const float Min, const float Max)
{
  if(Value < Min)
  {
    Value = Min;
  }
  else if(Value > Max)
  {
    Value = Max;
  }
  
  return Value;
}

float PTn_Sample(PTn_AttributesType* pThis, float Input)
{
  int i = 0;
  float Value = 0;

  //--- P part ---
  Value = (pThis->Kp * Input);

  //--- time constants ---
  for(i = 0; i < PTn_NO_OF_TIMECONSTS; i++)
  {
    Value = (pThis->TnValue[i] * pThis->Tn[i] + Value * pThis->Ta) / (pThis->Ta + pThis->Tn[i]);
    pThis->TnValue[i] = Value;
  }

  return Value;
}

float PID_Sample(PID_AttributesType* pThis, const float Input, const float StellwertOverflow)
{
  float Output = 0;
  float NewDT1Value = 0;

  //--- P part ---
  Output += pThis->PValue = (pThis->Kp * Input);

  //--- I part ---
  if(pThis->Ki == 0)
  {
    // Wenn der I-Anteil deaktiviert ist, soll dieser vollständig verschwinden.
    pThis->IValue = 0;
  }
  else
  {
    // Wenn nicht
    // - Anti-Windup enabled ist,
    // - der Stellwert in der Begrenzung ist 
    // - und der Fehler das selbe Vorzeichen hat...
    if(!(pThis->AntiWindup && (StellwertOverflow * Input > 0)))
    {
      // ...wird ein neuer I-Wert berechnet.
      pThis->IValue += pThis->Ki * pThis->Ta * Input;
    }
    else
    {
      // Sonst wird eine Anti-Wind-Up Mechanismus angewendet,
      // der wenn der Overflow und der I-Anteil unterschiedliches Vorzeichen haben...
      if(StellwertOverflow * pThis->IValue < 0)
      {
        // ...den I-Anteil reduziert bis dieser 0 ist...
        pThis->IValue += StellwertOverflow;
        if(StellwertOverflow * pThis->IValue > 0)
        {
            pThis->IValue = 0;
        }
      }
      else
      {
          // ...und sonst den I-Anteil eingefriert.
      }
    }
    Output += pThis->IValue;
  }

  //--- D part ---
#if 1
  NewDT1Value = (pThis->DT1Value * pThis->DT1 + Input * pThis->Ta) / (pThis->Ta + pThis->DT1);    // T1 of DT1
  Output += pThis->DValue = (pThis->Kd * (NewDT1Value - pThis->DT1Value) / pThis->Ta);     // D of DT1
  pThis->DT1Value = NewDT1Value; // save for next run
#else
  Output += pThis->DValue = (pThis->Kd*Input - pThis->Ta*pThis->DValue) / (pThis->Ta + pThis->DT1);
#endif    
  pThis->Output = Output;
  return Output;
}


//=============================================================================
// signal generator
//=============================================================================

typedef struct SollStoerGeneratorType
{
  unsigned int Period;
  float SollMin;
  float SollMax;
  float StoerMin;
  float StoerMax;
  void (*pTrigger)(int);

  // private
  unsigned int SampleNumber;
  char Phase;
}SollStoerGeneratorType;

void SollStoerGenerator_Sample (SollStoerGeneratorType* pThis, float* pSoll, float* pStoer)
{
//    float SollValue = pThis->SollMin;
//    float StoerValue = pThis->StoerMin;
  pThis->SampleNumber++;

  switch(pThis->Phase)
  {
    case 0: // Sollwert-Plus-Sprung
      *pSoll = pThis->SollMax;
      *pStoer = pThis->StoerMin;
      if(pThis->SampleNumber < pThis->Period/4)
        break;
      if(0 != pThis->pTrigger) pThis->pTrigger(pThis->Phase);
        //    sprintf (pThis->OutputText, "Soll+ E²t=%f",E2t_Fehler_aktuell;
        //    E2t_Fehler_aktuell = 0;
        pThis->Phase++;
        // fall through
    case 1: // Stoergroessen-Plus-Sprung
      *pSoll = pThis->SollMax;
      *pStoer = pThis->StoerMax;
      if(pThis->SampleNumber < pThis->Period/2)
          break;
      if(0 != pThis->pTrigger) pThis->pTrigger(pThis->Phase);
        // sprintf (pThis->OutputText, "Stoer+ E²t=%f",E2t_Fehler_aktuell;
        // E2t_Fehler_aktuell = 0;
      pThis->Phase++;
      // fall through
    case 2: // Sollwert-Minus-Sprung
      *pSoll = pThis->SollMin;
      *pStoer = pThis->StoerMax;
      if(pThis->SampleNumber < pThis->Period*3/4)
          break;
      if(0 != pThis->pTrigger) pThis->pTrigger(pThis->Phase);
      //    sprintf (pThis->OutputText, "Soll- E²t=%f",E2t_Fehler_aktuell;
      //    E2t_Fehler_aktuell = 0;
      pThis->Phase++;
      // fall through
    case 3: // Stoergroessesn-Minus-Sprung
      *pSoll = pThis->SollMin;
      *pStoer = pThis->StoerMin;
      if(pThis->SampleNumber < pThis->Period)
          break;
      if(0 != pThis->pTrigger) pThis->pTrigger(pThis->Phase);
      //    sprintf (pThis->OutputText, "Stoer- E²t=%f",E2t_Fehler_aktuell;
      //    E2t_Fehler_aktuell = 0;
      pThis->Phase++;
      // fall through
    default:
      pThis->Phase = 0;
      *pSoll = pThis->SollMin;
      *pStoer = pThis->StoerMin;
      // Es wurde bereits eine ganze Periode ausgegeben.
      pThis->SampleNumber = 0;
  }
}



#include <math.h>

enum  SignalWaveform
{
  SignalWaveformRect,
  SignalWaveformTriangle,
  SignalWaveformSine
};


typedef struct SignalGeneratorType
{
  enum SignalWaveform Waveform;
  unsigned int Period;
  float Min;
  float Max;
  void (*pTrigger)(int);

  // private
  unsigned int SampleNumber;
}SignalGeneratorType;

float SignalGenerator_Sample (SignalGeneratorType* pThis)
{
  float Value = pThis->Min;
  float Gradient = 0;

  if(pThis->SampleNumber == pThis->Period/2)
  {
    // Es wurde bereits eine halbe Periode ausgegeben.
    if(0 != pThis->pTrigger) pThis->pTrigger(0);
  }

  if(pThis->SampleNumber >= pThis->Period)
  {
    // Es wurde bereits eine ganze Periode ausgegeben.
    if (0 != pThis->pTrigger) pThis->pTrigger(1);
    pThis->SampleNumber = 0;
  }

  switch (pThis->Waveform)
  {
    case SignalWaveformRect:
      if (pThis->SampleNumber < (pThis->Period / 2))
      {
        Value = pThis->Max; // 1. Hälfte Max; 2. Hälfte Min
      }
      break;

    case SignalWaveformTriangle:
      Gradient = (pThis->Max - pThis->Min) / (pThis->Period /2);
      if(pThis->SampleNumber < (pThis->Period / 2))
      {
        Value = pThis->Min+Gradient * pThis->SampleNumber; // Anstieg
      }
      else if(pThis->SampleNumber < pThis->Period)
      {
        Value = pThis->Max-Gradient * (pThis->SampleNumber-pThis->Period/2); // Abfall
      }
      break;

    case SignalWaveformSine:
      Value = (pThis->Max - pThis->Min) / 2 * sin(2*3.141592 * pThis->SampleNumber / pThis->Period) + (pThis->Max + pThis->Min) / 2;
      break;
  }
  pThis->SampleNumber++;
  return Value;
}

//=============================================================================
// platform and application specific control part
//=============================================================================

PTn_AttributesType Istwertglaettung;
PTn_AttributesType Sollwertglaettung;
PID_AttributesType Regler;
PID_AttributesType Sollwertaufschaltung;
PID_AttributesType Steuerung;
PTn_AttributesType Strecke;

float Ta = 2*1E-3;
float Sollwert = 0;
int open_loop = 0;
float Istwert = 0;
float StellwertOverflow = 0;
float Aussentemp = 0;
//float Stoergroesse2 = 0;
//float Stoergroesse3 = 0;

// Gütemaße: Fehler-Quadrat und Fehler-Quadrat-mal-Zeit 
float E2t_Fehler_Output = 0;
float E2t_Fehler_aktuell = 0;
float E2_Fehler_Output = 0;
float E2_Fehler_aktuell = 0;
// Überschwingweite
// Ausregelzeit_3Prozent // bis innerhalb von 3%
// Anregelzeit // bis Sollwert erstmals erreicht

SignalGeneratorType SollwertGenerator;
SignalGeneratorType AussentemperaturTag;
SignalGeneratorType AussentemperaturJahr;
SollStoerGeneratorType SollStoerGenerator;

#include <sensoren.h>
#include <SvVis3.h>
#include <ARMV10_STD.h>
#include <control.h>
#include <steller.h>

#include <string.h>

/*
Sub-D 25 Pining:        Sub-D Pin  2 LED0

                                           |\
 Poti    ! SW0 ADC12_IN0 TIM2_CH2  PA0 --1-|o \
                                           |  o|-14-- PA1  TIM2_CH2 ADC12_IN1 SW1 ! Sollwert Input
Außentmp ! LED0          TIM4_CH3  PB8 --2-|o  |                                  
                                           |  o|-15-- PA3  TIM2_CH4 ADC12_IN3 SW3 !
         ! LED1          TIM4_CH4  PB9 --3-|o  |
                                           |  o|-16-- PA2  TIM2_CH3 ADC12_IN2 SW2 !
           LED2                    PB10--4-|o  |
                                           |  o|-17-- PA10 RxD
           LED3                    PB11--5-|o  |
                                           |  o|-18-- +3.3V
           LED4          TIM1_BKIN PB12--6-|o  |
                                           |  o|-19-- PA9  TxD
           LED5          TIM1_CH1N PB13--7-|o  |
                                           |  o|-20-- +5V
           LED6          TIM1_CH2N PB14--8-|o  |
                                           |  o|-21-- PB6  TIM4_CH1               ! Stellgroesse
           LED7          TIM1_CH3N PB15--9-|o  |
                                           |  o|-22-- PB7  TIM4_CH2               ! Sollwert Output
           SW6 ADC12_IN6 TIM3_CH1  PA6 -10-|o  |
                                           |  o|-23-- PB1  TIM3_CH4 ADC12_IN9 ADC ! Istwert
           SW7 ADC12_IN7 TIM3_CH2  PA7 -11-|o  |
                                           |  o|-24-- PC13
           SW5 ADC12_IN5           PA5 -12-|o  |
                                           |  o|-25-- Gnd
           SW4 ADC12_IN4           PA4 -13-|o /
                                           |/
*/

void Update_Control()
{
  float Abweichung = 0;
  float Stellwert = 0;
  const float min_temp = -20; // 0V
  const float max_temp = 46; // 3,3V

  LED3 = 1; // runtime indication

  //--- Signalgeneratoren ---
//    Sollwert = SignalGenerator_Sample (&SollwertGenerator);
  Aussentemp = SignalGenerator_Sample(&AussentemperaturTag) + SignalGenerator_Sample(&AussentemperaturJahr);
  SollStoerGenerator_Sample(&SollStoerGenerator, &Sollwert, &Aussentemp);

  //--- Sensoren/Inputs: ADC 0-4095, Linearisierung und Umrechnung (z.B. ADC[V] -> % oder ?C) ---
  // 12bit 0-4095 convertet to 100%
//    Sollwert = Get_ADC_PA0()*100.0/4096; // Sub-D Pin 1 (für Eingang von Poti neben CPU)
//    Sollwert = Get_ADC_PC4()*100.0/4096; // poti next to CPU
//    Sollwert = Get_ADC_PA1()*100.0/4096; // Sub-D Pin 14: für Eingang von Funktionsgenerator
//    Sollwert =  Sollwert*((max_temp-min_temp)/100)+min_temp;
//    Sollwert = 20; // konstanter Sollwert

  Istwert = Get_ADC_PB1()*100.0/4096; // Sub-D Pin 23
  Istwert = Istwert*((max_temp-min_temp)/100)+min_temp;
  
//    Aussentemp = Get_ADC_PA1()*100.0/4096; // Sub-D Pin 14
//    Stoergroesse2 = Get_ADC_PA2()*100.0/4096; // Sub-D Pin 16
//    Stoergroesse3 = Get_ADC_PA3()*100.0/4096; // Sub-D Pin 15

  //--- Soll- und Istwertglaettung ---
  Sollwert = PTn_Sample(&Sollwertglaettung, Sollwert);
  Istwert = PTn_Sample(&Istwertglaettung, Istwert);

  //--- Fehlerberechnung ---
  Abweichung = Sollwert - Istwert;
  E2_Fehler_aktuell += Abweichung * Abweichung;
  E2t_Fehler_aktuell += Abweichung * Abweichung * SollwertGenerator.SampleNumber / SollwertGenerator.Period;
  if (open_loop) Abweichung = Sollwert;

  //--- Regler ---
  Stellwert += PID_Sample (&Regler, Abweichung, StellwertOverflow);

  //--- Sollwertaufschaltung & Stoergroessenaufsteuerung ---
  Stellwert += PID_Sample(&Sollwertaufschaltung, Sollwert, 0);
  Stellwert += PID_Sample(&Steuerung, Sollwert - Aussentemp, 0);

  //--- Stellgrößenbeschränkung ---
  StellwertOverflow = Stellwert;
  Stellwert = limit_value(Stellwert, min_temp, max_temp);
  if (!open_loop) Stellwert = limit_value(Stellwert, Istwert, max_temp);// Heizung kann nicht kühlen
  StellwertOverflow -= Stellwert;

  //--- Simulation der Strecke ---
//    Istwert = PTn_Sample(&Strecke, Stellwert);

  //--- Steller/Outputs: Umrechnung (?C -> %[V]), PWM 0-100 % ---
  Set_PWM_PB7((Sollwert-min_temp)/((max_temp-min_temp)/100)+0.5); // Sub-D Pin 22
  Set_PWM_PB6((Stellwert-min_temp)/((max_temp-min_temp)/100)+0.5); // Sub-D Pin 21 
  //Set_PWM_PB6(Stellwert); // Sub-D Pin 21 
//    Set_PWM_PB9(Stellwert); // Sub-D Pin  3 LED1
//    Set_Channel_1(Stellwert);        // Ausgabe Stellgroesse 0-100%
  Set_PWM_PB8((Aussentemp-min_temp)/((max_temp-min_temp)/100)+0.5); // Sub-D Pin  2 LED0

  /* ------------- Update SvVis3 Visulalization -------------*/
  SvVis3_Write_I16(1, (int16_t) (Sollwert*100));
  SvVis3_Write_I16(2, (int16_t) (Istwert*100));
  SvVis3_Write_I16(3, (int16_t) (Stellwert*100));
  SvVis3_Write_I16(4, (int16_t) (Aussentemp*100));
  SvVis3_Write_I16(5, (int16_t) (Regler.IValue*100));
  SvVis3_Write_I16(6, (int16_t) (Regler.DValue*100));

  LED3 = 0; // runtime indication
}


//#define LED3  *((volatile unsigned long *)(BITBAND_PERI(GPIOB_ODR,11)))   // PB11

void Trigger_LED3 (int Value)
{
  LED3 = Value;
}

char OutputText[30] = "";

void Trigger_E2t_Fehler(int Value)
{
  switch(Value)
  {
  case 0:
      E2t_Fehler_Output = E2t_Fehler_aktuell;
      sprintf(OutputText, "Soll+ E²t=%f", E2t_Fehler_aktuell);
      break;
  case 1:
      sprintf(OutputText, "Stoer+ E²t=%f", E2t_Fehler_aktuell);
      break;
  case 2:
      sprintf(OutputText, "Soll- E²t=%f", E2t_Fehler_aktuell);
      break;
  case 3:
      sprintf(OutputText, "Stoer- E²t=%f", E2t_Fehler_aktuell);
      break;
  }
  E2t_Fehler_aktuell = 0;
}

void Trigger_E2_Fehler(int Value)
{
  if(Value ==1) // nur nach einer ganzen Periode
  {
    E2_Fehler_Output = E2_Fehler_aktuell;
    E2_Fehler_aktuell = 0;
  }
}

void RT_Init()
{
  memset(&Regler, 0, sizeof(Regler));
  Regler.Ta = Ta;
  Regler.Kp = 2;
//    Regler.Ki = 0; // Ki=Kp/Tn
//    Regler.AntiWindup = 1;
//    Regler.DT1 = 4E-3;
//    Regler.Kd = 0; // Kd=Tv/Kp

  memset(&Istwertglaettung, 0, sizeof(Istwertglaettung));
  Istwertglaettung.Ta = Ta;
  Istwertglaettung.Kp = 1;
//    Istwertglaettung.Tn[0]=4E-3;

  memset(&Sollwertglaettung, 0, sizeof(Sollwertglaettung));
  Sollwertglaettung.Ta = Ta;
  Sollwertglaettung.Kp = 1;
//    Sollwertglaettung.Tn[0]=4E-3;

  memset(&Sollwertaufschaltung, 0, sizeof(Sollwertaufschaltung));
  Sollwertaufschaltung.Ta = Ta;
  Sollwertaufschaltung.Kp = 1;
  Sollwertaufschaltung.Kd = 0;

  memset(&Steuerung, 0, sizeof(Steuerung));
  Steuerung.Ta = Ta;
  Steuerung.Kp = 0;//0.5;
  Steuerung.Kd = 0;

  memset(&Strecke, 0, sizeof(Strecke));
  Strecke.Ta = Ta;
  Strecke.Kp = 0.5;
  Strecke.Tn[0]=10E-3;
  Strecke.Tn[1]=100E-3;

  memset(&SollwertGenerator, 0, sizeof(SollwertGenerator));
//    SollwertGenerator.Period = 1/Ta; // 1sec bei 1kHz = 1000 Samples;
  SollwertGenerator.Period = 2*2.4*500; // (1sec bei 500Hz = 500 Samples);   2 Tage statt 7
  SollwertGenerator.Waveform = SignalWaveformRect;
  SollwertGenerator.Min = -10; //15; vorher -20
  SollwertGenerator.Max = 10; //20; vorher 46
  SollwertGenerator.pTrigger = &Trigger_E2t_Fehler;

  memset(&SollStoerGenerator, 0, sizeof(SollStoerGenerator));
//    SollStoerGenerator.Period = 1/Ta; // 1sec bei 1kHz = 1000 Samples;
  SollStoerGenerator.Period = 4*2.4*500; // (1sec bei 500Hz = 500 Samples);
  SollStoerGenerator.SollMin = 15;
  SollStoerGenerator.SollMax = 20;
  SollStoerGenerator.StoerMin = -10; //15;
  SollStoerGenerator.StoerMax = 10; //20;
  SollStoerGenerator.pTrigger = &Trigger_E2t_Fehler;

  memset(&AussentemperaturTag, 0, sizeof(AussentemperaturTag));
//    AussentemperaturTag.Period = 1/Ta; // 1sec bei 1kHz = 1000 Samples;
  AussentemperaturTag.Period = 2.4*500; // (1sec bei 500Hz = 500 Samples);
  AussentemperaturTag.Waveform = SignalWaveformSine;
//    AussentemperaturTag.Min = -10;
//    AussentemperaturTag.Max = +10;
//    AussentemperaturJahr.pTrigger = &Trigger_E2_Fehler;

  memset(&AussentemperaturJahr, 0, sizeof(AussentemperaturJahr));
  AussentemperaturJahr.Period = 10 * AussentemperaturTag.Period;
  AussentemperaturJahr.Waveform = SignalWaveformSine;
//    AussentemperaturJahr.Min = -10;
//    AussentemperaturJahr.Max = 30;
//    AussentemperaturJahr.pTrigger = &Trigger_E2_Fehler;
}

/*****************************************************************************/
/*       Temperaturreglung mit digitalem Regler: Main Programm               */
/*****************************************************************************/
int main(void)
{
  __IO uint16_t lcd_soll = 0;
  __IO uint16_t lcd_ist = 0;
  char x = 0;
  char buffer[100];

  SystemInit();         // SystemTakt auf 72MHz
  SvVis3_Init();        // Init System Visualisation
  init_leds_switches(); // Initialisiere LED Schalter Platine
  dil_led_init();       // Initialisiere LED`s DIL Adapter

  lcd_init();          // LCD Initialisieren
  lcd_clear();
  set_leds(0);         //Leds auf 0 setzen

  Init_Stellglied();

  DMA_Config();        // Initialisiert DMA Controller, um ADC Sensor Werte kontinuierlich in den Speicher zu kopieren
  SensorInit();        // Initialisiert ADC Inputs für Sensoren

  /* -------------- SysTick - Timer initialisieren ------------------*/
  SystemCoreClockUpdate();
  SysTick_Config(2*SystemCoreClock/1000);         // 2*1ms Periodendauer
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

  /* ------------- NVIC initialisieren von SysTick Timer ------------*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  NVIC_SetPriority(SysTick_IRQn, 14); //SysTick_IRQn = -1

  //--- control engineering init ----
  RT_Init();

  while(1)
  {
    set_leds(x);                         // Running LEDS every 400 ms
    x++;
    wait_ms(1000);
    if((lcd_soll != Sollwert) || (lcd_ist != Istwert))     // Values Changed ?
    {
      lcd_soll = Sollwert;
      lcd_ist = Istwert;

      // Display: 16 chars in 2 lines

      lcd_set_cursor(0,0);
      sprintf(&buffer[0], "%2.1f-%2.1f=%2.1f", Sollwert, Istwert, (Sollwert-Istwert));
      lcd_put_string(&buffer[0]);

      lcd_set_cursor(1,0);
/*    if(E2t_Fehler_Output >0)
          sprintf(&buffer[0], "E2t_Fehler: %.2f", E2t_Fehler_Output);
      else if (E2_Fehler_Output >0)
          sprintf(&buffer[0], "E2_Fehler: %.2f", E2_Fehler_Output);
      else*/
#if 0
      sprintf(&buffer[0], "%2.0fP%2.0fI%2.0fD%2.0f@%2.1f",\
      Regler.PValue, Regler.IValue, Regler.DValue, Regler.Output, Aussentemp);
      lcd_put_string(&buffer[0]);
#else
      lcd_put_string(OutputText);
#endif
    }
  }   /* Endless loop */
}  /* End main */
