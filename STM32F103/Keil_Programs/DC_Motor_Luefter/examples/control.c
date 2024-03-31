#include <stdlib.h>
#include <stdint.h>
#include <control.h>

void regler(float temp, float tempmax, float tempmin, uint8_t state)
{
  error = -tempmin + temp; // aktuelle Sollabweichung
  if(state == 3)          // falls PI Regler
  {
    ersum += error;  // Integralteil: Sollabweichung aufaddieren
    if(ersum > 100) // Begrenzung des I Tei ls
    {
      ersum = 100;
    }
  }

  pwm = (255 / (tempmax - tempmin)) * temp - (255 / (tempmax - tempmin)) * tempmin + ersum; // P/PI Regler

  if((255 / (tempmax - tempmin)) * temp - (255 / (tempmax - tempmin)) * tempmin + ersum > 255) // falls PWM Wert ueber
  {
    pwm = 255; // PWM Wert Be grenzung
  }

  if(temp > tempmin && temp < tempmax) // falls T_min < T < T_max
  {
    PORTB = (1 << PB0);                      // gelbe LED anschalten
    OCR1A = pwm;                             // weise PWM Register Reglerwert zu
    dtostrf(pwm * 100 / 255, 3, 0, Buffer2); // Schreibe PWM Tastgrad in String Buffer2
  }

  if(temp <= tempmin) // falls T<T_min
  {
    ersum = 0;                   // setze I Teil zurueck
    PORTB = (1 << PB1);          // gruene LED anschalten
    pwm = 0;                     // PWM Tastgrad auf 0% setzen
    OCR1A = pwm;                 // PWM Register zuweisen
    dtostrf(pwm, 3, 0, Buffer2); // Schreibe PWM Tastgrad in String Buffer2
  }

  if(temp >= tempmax) // falls T>T_max
  {
    PORTB = (1 << PB2);                      // rote LED anschalten
    pwm = 255;                               // PWM Tastgrad auf Maximun
    OCR1A = pwm;                             // PWM Register zuweisen
    dtostrf(pwm * 100 / 255, 3, 0, Buffer2); // Schreibe PWM Tastgrad in String Buffer2
  }

  if(state == 1) // falls Luefterregelung aktiv
  {
    OCR1A = konstpwm;                             // weise PWM Register manuellen Wert zu
    dtostrf(konstpwm * 100 / 255, 3, 0, Buffer2); // Schreibe PWM Tastgrad in String Buffer2
  }
}
