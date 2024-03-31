#ifndef REGLER_H
#define REGLER_H

#include <stdint.h>

extern volatile float tempmax; // obere Grenztemp
extern volatile float tempmin; // untere Grenztemp
extern volatile uint8_t state; // Zustand
extern volatile uint8_t zeile;
extern volatile uint8_t konstpwm;  // konstante Lüfterdrehzahl
extern volatile float ersum;       // Integralteil des Reglers
extern volatile uint8_t edit_flag; // Optionsmenue Flag
extern char Buffer2[30];
extern float temp;
extern uint8_t pwm;
extern float error;

void regler(float temp, float tempmax, float tempmin, uint8_t state);

#endif // REGLER_H
