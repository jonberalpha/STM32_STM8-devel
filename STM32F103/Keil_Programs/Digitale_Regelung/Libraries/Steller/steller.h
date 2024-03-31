/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   Steller.c                                                     */
/* Autor:       Josef Reisinger                                               */
/* Version:     V1.00                                                         */
/* Date:        07/02/2016                                                    */
/* Description: Ansteuerung des Stellgliedes des Regler mittles PWM Signal    */
/*              PWM Signal wird mit Timer 4 erzeugt                           */  
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
/*  PWM - Signals:                                                            */
/*  PB6 -> PWM Signal Channel 1 of Timer 4                                    */
/*  PB7 -> PWM Signal Channel 2 of Timer 4                                    */
/*  PB8 -> PWM Signal Channel 4 of Timer 4                                    */
/*  PB9 -> PWM Signal Channel 4 of Timer 4                                    */
/******************************************************************************/

/* ------------------Define to prevent recursive inclusion -------------------*/
#ifndef __STELLER_H
#define __STELLER_H


#ifdef STELLER_MOD
#define EXPORT
#else
#define EXPORT extern
#endif 

//  Initialisiert Portleitungen für Auadrocopter-Motoren
EXPORT void Init_Stellglied(void);
EXPORT void Set_Channel_1(char dutycycle);

// ZOG: added
EXPORT void Set_PWM_PB6(char dutycycle);
EXPORT void Set_PWM_PB7(char dutycycle);
EXPORT void Set_PWM_PB8(char dutycycle);
EXPORT void Set_PWM_PB9(char dutycycle);

#undef  EXPORT
#endif /* STELLER_MOD  */

