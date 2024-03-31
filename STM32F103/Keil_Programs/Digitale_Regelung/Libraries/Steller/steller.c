/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2009-2009 All rights reserved. AUSTRIA   */ 
/*                                                                            */ 
/* File Name:   Steller.c                                                     */
/* Autor:       Josef Reisinger, Gerald Zottl                                 */
/* Version:     V1.10                                                         */
/* Date:        07/02/2016                                                    */
/* Description: Ansteuerung des Stellgliedes des Regler mittles PWM Signal    */
/*              PWM Signal wird mit Timer 4 erzeugt                           */  
/******************************************************************************/
/* History:   V1.00  REJ: creation                                            */
/*            V1.10  ZOG: make PB7-PB9 work; all @ 20kHz                      */
/******************************************************************************/
/*  PWM - Signals:                                                            */
/*  PB6 -> PWM Signal Channel 1 of Timer 4                                    */
/*  PB7 -> PWM Signal Channel 2 of Timer 4                                    */
/*  PB8 -> PWM Signal Channel 4 of Timer 4                                    */
/*  PB9 -> PWM Signal Channel 4 of Timer 4                                    */
/******************************************************************************/
#define STELLER_MOD
#include <steller.h>
#include <stm32f10x_tim.h>


int motorspeed_1=1000;    // Motorspeed bei Motor 1  , bei Start (Motor aus)
int motorspeed_2=1000;    // Motorspeed bei Motor 2
int motorspeed_3=1000;    // Motorspeed bei Motor 3
int motorspeed_4=1000;    // Motorspeed bei Motor 4

static uint16_t PWM_Periode = 50; // ZOG: 50탎 = 1/20kHz

/*****************************************************************************/
/*                        INIT TIMER 4 to create PWM Signals                 */
/*****************************************************************************/
void Init_Stellglied(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
  TIM_OCInitTypeDef TIM_OC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // Clock des Ports einschalten
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /*------------------ Init GPIO B Pin 6-9 --------------*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

/*  ZOG: TIM1 doesn't work for what ever reason. Therefore commented out again.
    //------------------ Init GPIO B Pin 12-15 --------------
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
*/
  TIM_DeInit(TIM4);
//  TIM_DeInit(TIM1);

  /*------------------ Init TimeBase --------------*/
  TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  // TIM_Prescaler configures the prescaler value used to divide the TIM clock. This member
  // must be a number between 0x0000 and 0xFFFF.
  TIM_TimeBase_InitStructure.TIM_Prescaler = 71; // 71 at 72MHz SysTick is 1MHz => 1탎
  // TIM_Period configures the period value to be loaded into the active Auto-Reload Register at
    // the next update event. This member must be a number between 0x0000 and 0xFFFF.
  TIM_TimeBase_InitStructure.TIM_Period = PWM_Periode - 1; // 49 at 1탎 is 50탎 => 20kHz
  TIM_TimeBaseInit(TIM4, &TIM_TimeBase_InitStructure);
//  TIM_TimeBaseInit(TIM1, &TIM_TimeBase_InitStructure);

  /*------------------ Init Output Compare Einheit --------------*/
  TIM_OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OC_InitStructure.TIM_Pulse = 1000;
  TIM_OC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High ;
/*  // TIM1 uses N-Values
  TIM_OC_InitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OC_InitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; //Low
  TIM_OC_InitStructure.TIM_OCIdleState = TIM_OCIdleState_Set ; //Reset
  TIM_OC_InitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //Set
*/

  /*------------------ Init Output Compare Einheit --------------*/
  TIM_OC1Init(TIM4, &TIM_OC_InitStructure); // CH1 -> PB6
  TIM_OC2Init(TIM4, &TIM_OC_InitStructure); // CH2 -> PB7
  TIM_OC3Init(TIM4, &TIM_OC_InitStructure); // CH3 -> PB8
  TIM_OC4Init(TIM4, &TIM_OC_InitStructure); // CH4 -> PB9

/*  TIM_OC1Init(TIM1, &TIM_OC_InitStructure); // CH1 -> PB13
  TIM_OC2Init(TIM1, &TIM_OC_InitStructure); // CH2 -> PB14
  TIM_OC3Init(TIM1, &TIM_OC_InitStructure); // CH3 -> PB15
//  TIM_OC4Init(TIM1, &TIM_OC_InitStructure); // CH4 -> PB12?? TIM1_BKIN
*/
  
  TIM_Cmd(TIM4, ENABLE);
//  TIM_Cmd(TIM1, ENABLE);
}


/*****************************************************************************/
/*               Set duty cycle Channel 1                                    */
/*****************************************************************************/
void Set_Channel_1(char dutycycle)
{
//  TIM_SetCompare1 (TIM4, dutycycle/* *100*/);
  TIM_SetCompare1(TIM4, (((unsigned int)dutycycle) * PWM_Periode)/100);
}

// ZOG: added

// TIM4_CH1 -> PB6
void Set_PWM_PB6(char dutycycle)
{
  unsigned int uiValue = dutycycle;
  TIM_SetCompare1(TIM4, (uiValue * PWM_Periode)/100);
}

// TIM4_CH2 -> PB7
void Set_PWM_PB7(char dutycycle)
{
  unsigned int uiValue = dutycycle;
  TIM_SetCompare2(TIM4, (uiValue * PWM_Periode)/100);
}

// TIM4_CH3 -> PB8 (LED0)
void Set_PWM_PB8(char dutycycle)
{
  unsigned int uiValue = dutycycle;
  TIM_SetCompare3(TIM4, (uiValue * PWM_Periode)/100);
}

// TIM4_CH4 -> PB9 (LED1)
void Set_PWM_PB9(char dutycycle)
{
  unsigned int uiValue = dutycycle;
  TIM_SetCompare4(TIM4, (uiValue * PWM_Periode)/100);
}

/*
void Set_PWM_PB13(char dutycycle)
{
  TIM_SetCompare1(TIM1, (((unsigned int)dutycycle) * PWM_Periode)/100);
}
void Set_PWM_PB14(char dutycycle)
{
  TIM_SetCompare2(TIM1, (((unsigned int)dutycycle) * PWM_Periode)/100);
}
void Set_PWM_PB15(char dutycycle)
{
  TIM_SetCompare3(TIM1, (((unsigned int)dutycycle) * PWM_Periode)/100);
}
*/
/*
void Set_PWM_PB12(char dutycycle)
{
  TIM_SetCompare4(TIM1, (((unsigned int)dutycycle) * PWM_Periode)/100);
}
*/

