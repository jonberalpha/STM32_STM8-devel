//
//  (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   
//                                                                            
//   File Name:  control.h                                                   
//   Autor:       Josef Reisinger 
//   Version:     V1.00                                                        
//   Date:       07/02/2016                                                    
//   Description:   Reglersoftware für Temperaturregelung                                            
//
//   History:     V1.00 creation
//

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_H
#define __CONTROL_H

/* ------------------------------ Regler Konstanten ----------------------*/
#if 0 // zog
#define Ta        0.001 //0,2 //1       // sample Period 1s
#define Kp        100 //0.45    // Proportionalteil für Regler 
#define Ki        0.0     // Integralteil für Regler 
#define Kd        0.0      // Differentialteil für Regler 
// zog #define TN         0.5     // Nachstellzeit für Regler

/* ------------------------------- Wertebereiche ------------------------*/
#define PWM_MIN 0
#define PWM_MAX 0x0FF
#endif // zog


#ifdef CONTROL_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

EXPORT float sollwert;
EXPORT float istwert;
//zog
EXPORT float stoerwert;

EXPORT void Update_Control(void);

#undef  EXPORT

#endif

