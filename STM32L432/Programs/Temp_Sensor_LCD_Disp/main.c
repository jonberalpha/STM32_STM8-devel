/*______________________________ INCLUDES ___________________________________*/
#include <stm32f10x.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "DELAY_STM32F10X.h"
#include "LCD_I2C.h"
#include "SENS_DS18X20.h"
#include "UART_STM32F10X.h"

/*___________________________ GLOBAL VARIABLES ______________________________*/
volatile char line_buffer[LINEMAX + 1]; // Holding buffer with space for terminating NULL
volatile int line_valid = 0;

/*_________________________ PRIVATE PROTOTYPES ______________________________*/
static void rgb_led_init(void);

// UART RX ISR
void USART1_IRQHandler(void)
{ 
	// pending-bit wird automatisch durch Lesen des UART Datenregister (USART1->DR) zurückgesetzt
	
	static char rx_buffer[LINEMAX]; // buffer to build line
	static int rx_index = 0;
	
	char rx = uart_get_char();
	if((rx == '\r') || (rx == '\n')) // end-of-line condition?
	{
		if(rx_index != 0) // content?
		{
			memcpy((void *)line_buffer, rx_buffer, rx_index); // Copy from dynamic receive buffer to line buffer 
			line_buffer[rx_index] = 0; // Add terminating NULL or \0
			line_valid = 1;            // flag new line valid for processing
			rx_index = 0;              // Reset content pointer
		}
	}
	else
	{
		if(rx_index == LINEMAX) // if overflow back to start
		{
			rx_index = 0;
		}
		rx_buffer[rx_index++] = rx; // copy to buffer and increment
	}
}

static void rgb_led_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // enable GPIOA CLK
	GPIOA->CRL &= ~0xFFF; // Config-Bits loeschen
	GPIOA->CRL |= 0x333;  // all to GPOPP
}

int main(void)
{
	float temp=0, diff = 0, solltemp = 22.0;
	char buffer[20];
	
	rgb_led_init();
	uart_init(9600);
	uart_clear();
	
	lcd_i2c_init(); // PB8 - SCL, PB9 - SDA
  lcd_i2c_clear();
	lcd_i2c_put_curs(0, 0);
	
	DS18X20_init(); // Temp-Sensor on PB0
	
	// switch RGB-LED completely off
	LEDr = 0;
	LEDg = 0;
	LEDb = 0;
	
	while(1)
	{
		if(line_valid == 1) // correct rx uart line received
		{
			solltemp = (float)atof((char *)line_buffer); // if uart sent line convert to new soll-temperature, 0 is assigned on failure
		}
		temp = get_temperature();
		lcd_i2c_put_curs(0, 0);
		sprintf(buffer, "IST-Temp:%6.2f", temp);
		lcd_i2c_send_string(buffer);
		lcd_i2c_put_curs(1, 0);
		sprintf(buffer, "Soll-Temp:%5.1f", solltemp);
		lcd_i2c_send_string(buffer);
		
		diff = solltemp - temp; // absolute difference
		// diff = (solltemp - temp)/temp * 100; // difference in percent
		if(diff < 0) // abs of difference
		{
			diff = diff * -1;
		}
		
		if(diff <= 3) // at 3 inner tolerance
		{
			if(diff <= 1) // at 1 inner-inner tolerance
			{
				LEDr = 0;
				LEDg = 1; // full green
				LEDb = 0;
			}
			else
			{
				LEDr = 1; // mix of red and green
				LEDg = 1;
				LEDb = 0;
			}
		}
		else
		{
			LEDr = 1; // full red
			LEDg = 0;
			LEDb = 0;
		}
		
		delay_ms(5);
	}
}
