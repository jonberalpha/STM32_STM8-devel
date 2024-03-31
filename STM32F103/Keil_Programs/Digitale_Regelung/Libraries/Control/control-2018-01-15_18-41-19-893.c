/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   Control.c                                                     */
/* Autor:       Josef Reisinger                                               */
/* Version:     V1.00                                                         */
/* Date:         07/02/2016                                                   */
/* Description: Controller Software für digital Controller                    */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#define CONTROL_MOD
#include "control.h"
#include "SvVis3.h"
#include "ARMV10_STD.h"


/*------------------------------ Function Prototypes ----------------*/
void Update_Control(void);   

/* ----------------- Werte für Temperaturregleung ------------------------*/
static float istalt = 0;
static float istneu = 0;
static float Output = 0;

static float e, esum=0, ealt=0;

// zog begin
#include "steller.h"
//#include "stm32f10x.h"
//#include "stm32f10x_map.h"
/* ------------------------------ Regler Konstanten ----------------------*/
static float Ta=0.001; // 0.2 //1       // sample Period 1s
static float Kp=5; //0.45    // Proportionalteil für Regler 
static float Ki=0.5;        // 0.0     // Integralteil für Regler 
static float Kd=0; //0.0      // Differentialteil für Regler 
// zog #define TN         0.5     // Nachstellzeit für Regler

/* ------------------------------- Wertebereiche ------------------------*/
static unsigned char PWM_MIN=0;
static unsigned char PWM_MAX=0x0FF;


/*
Sub-D 25 Pin-Belegung:

        |\
PA0 --1-|o \
        |  o|-14-- PA1
PB8 --2-|o  |
        |  o|-15-- PA3
PB9 --3-|o  |
        |  o|-16-- PA2
PB10--4-|o  |
        |  o|-17-- PA10 RxD
PB11--5-|o  |
        |  o|-18-- +3.3V
PB12--6-|o  |
        |  o|-19-- PA9 TxD
PB13--7-|o  |
        |  o|-20-- +5V
PB14--8-|o  |
        |  o|-21-- PB6 PWM Stellgröße
PB15--9-|o  |
        |  o|-22-- PB7
PA6 -10-|o  |
        |  o|-23-- PB1 ADC Istwert
PA7 -11-|o  |
        |  o|-24-- PC13
PA5 -12-|o  |
        |  o|-25-- Gnd
PA4 -13-|o /
        |/
*/
// zog end



/*****************************************************************************/
/*                        Update Control Values                              */
/*****************************************************************************/
void Update_Control() {   

// ZOG beginn
  static int Zahl = 0;
  int Counts = 0; // 5; //0;
  static int Rampe = 0;
// ZOG end


  static int count=0;
  int stellwert;

/*
    static unsigned int OldTick = 0;
  const unsigned int CurrentTick = SysTick_GetCounter();
  if (CurrentTick != OldTick +1)
  {
    // hoppala wir sind zu langsam
    OldTick=CurrentTick;
  }
  OldTick=CurrentTick;
  */


  count++;
  if(count>=Counts)
  { // Aktualisiere  Regler alle 1s
    count = 0;
    DIL_LED2 = ~DIL_LED2; // Heatbeat LED every 1 second
  
#if 1
//    stellwert = (sollwert*100)/4096; // Umrechnung des sollwert (0-4096) auf einen Wert zwischen 0-100% 
    stellwert = Update_PID();
#else  
  // ZOG beginn

    Rampe++;
    if (Rampe >=100) Rampe =0;
    stellwert = Rampe;

/*
    Zahl += 10;
    if (Zahl >= 100)
    {
      Zahl = 0;
    }
    stellwert = Zahl;
*/
//    stellwert = (istwert*100)/4096;
  // ZOG end
#endif  
    Set_Channel_1(stellwert);        // Ausgabe Stellgroesse 0-100%
  
    /* ------------- Update SvVis3 Visulalization -------------*/
    SvVis3_Write_I16(1, (int16_t) sollwert);
    SvVis3_Write_I16(2, (int16_t) istwert);
  }
}          

/*****************************************************************************/
/*                        Update PID                                         */
/*****************************************************************************/

// zog void Update_PID()
int Update_PID()
{
    int value;

  static float SollwertGlatt=0;
  float TSollwertGlatt=3;
  static float IstwertGlatt=0;
  float TIstwertGlatt=0.01;
  float IAnteilAlt = 0;

  // Sollwertglättung (PT1)
//  SollwertGlatt = SollwertGlatt * (Ta/TSollwertGlatt) + sollwert * (1-Ta/TSollwertGlatt);
  SollwertGlatt = (SollwertGlatt * TSollwertGlatt + sollwert * Ta) / (Ta + TSollwertGlatt);
  
  // Istwertglättung (PT1)
//  IstwertGlatt = IstwertGlatt * (Ta/TIstwertGlatt) + istwert * (1-Ta/TIstwertGlatt);
  IstwertGlatt = (IstwertGlatt * TIstwertGlatt + istwert * Ta) / (Ta + TIstwertGlatt);
  

//    e = istwert - sollwert;     // Calculate Error Signale
//    e = sollwert - istwert;     // Calculate Error Signale
    e = SollwertGlatt - IstwertGlatt;     // Calculate Error Signale
    /*----------------- Check Boundaries ------------------------*/
/* zog
    if(e >= (ealt + 100))
    {
        e=istalt - sollwert;
    }
    else
    {
        e=istwert - sollwert;
    }
zog */

    /*  ------------------ Calc new Control Value for PID Controller ---------------------*/
#if 0
  esum = esum + e;                                 // Sum up Error Signal over time
    Output=(Kp*e) + (Ki*Ta*esum) + (Kd*(e-ealt)/Ta);        // Equation of PID Controller
    ealt = e;                             // Store actual error signal for next step
    //if(esum > 10000)  {                                // Limit error sum
    //  esum=10000;
    //  }
    //else if(esum < (-10000))  {
    //  esum=-10000;
    //  }
#else
  if (Ki == 0)
  {
    IAnteilAlt = 0;
    Output = 0;
  }
  else
  {
    Output = IAnteilAlt + Ki*Ta*e;
  }

  Output += (Kp*e) + /*(Ki*Ta*esum) + */(Kd*(e-ealt)/Ta);
#endif

    /*------------------ Output new control Signal-----------*/
//    value = (int)(PWM_MIN+(Output/100.0)*(PWM_MAX-PWM_MIN));
  if (Output > 4096)
    value = 4096;
  else if (Output < 0)
    value = 0;
  else
      value = (int)Output;
  return value;
}
