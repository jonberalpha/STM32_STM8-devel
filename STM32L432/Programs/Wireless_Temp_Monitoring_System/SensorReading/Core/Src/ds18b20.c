/**
 *******************************************************************************
 * @file    ds18b20.c
 * @authors Berger, Ibrahim, Markovic, Meyerowitsch
 * @brief   This file provides code to communicate with the DS18B20 sensor via
 *          the 1-wire protocol
 ******************************************************************************
 */

#include "ds18b20.h"
#include "tim.h"
#include "main.h"
#include "gpio.h"
#include "cmsis_os.h"

// Private function prototypes
static void delay_us(uint32_t us);
static void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
static void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
static void DS18B20_Write (uint8_t data);
static uint8_t DS18B20_Read (void);

/**
 * @brief	Function that implements a delay in us
 * @param   (uint32_t) - Time in us
 * @retval	(none)
 */
static void delay_us(uint32_t us)
{
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    while((__HAL_TIM_GET_COUNTER(&htim1)) < us);
}

/**
 * @brief	Function that sets sensor pin as output
 * @param   (GPIO_TypeDef) - GPIO Port of the sensor
 * @param   (uint16_t) - Pin number of the sensor
 * @retval	(none)
 */
static void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/**
 * @brief	Function that sets sensor pin as input
 * @param   (GPIO_TypeDef) - GPIO Port of the sensor
 * @param   (uint16_t) - Pin number of the sensor
 * @retval	(none)
 */
static void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/**
 * @brief	Function that starts communication with the sensor
 * @retval	(uint8_t) - Returns response of the sensor
 */
static uint8_t DS18B20_Start(void)
{
	uint8_t Response = 0;

	Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);   // set the pin as output
	HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin low
	delay_us(480);   // delay_us according to datasheet

	Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);    // set the pin as input
	delay_us(80);    // delay_us according to datasheet

	if (!(HAL_GPIO_ReadPin (DS18B20_PORT, DS18B20_PIN))) Response = 1;    // if the pin is low i.e the presence pulse is detected
	else Response = -1;

	delay_us(400); // 480 us delay_us totally.

	return Response;
}

/**
 * @brief	Function that writes 1 Byte to the sensor
 * @param   (uint8_t) - 1 byte of data which should be written
 * @retval	(none)
 */
static void DS18B20_Write(uint8_t data)
{
	Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);  // set as output

	for (int i=0; i<8; i++)
	{

		if ((data & (1<<i))!=0)  // if the bit is high
		{
			// write 1

			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);  // set as output
			HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin LOW
			delay_us(1);  // wait for 1 us

			Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);  // set as input
			delay_us(50);  // wait for 60 us
		}

		else  // if the bit is low
		{
			// write 0

			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);
			HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin LOW
			delay_us(50);  // wait for 60 us

			Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);
		}
	}
}

/**
 * @brief	Function that reads 1 Byte from the sensor
 * @retval	(uint8_t) - Returns 1 Byte sensor data
 */
static uint8_t DS18B20_Read(void)
{
	uint8_t value=0;

	Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);

	for(int i = 0; i < 8; i++)
	{
		Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);        // set as output

		HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, 0);  // pull the data pin LOW
		delay_us(1);        // wait for > 1us

		Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);         // set as input
		if(HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN))   // if the pin is HIGH
		{
			value |= 1<<i;  // read = 1
		}
		delay_us(50);       // wait for 50 us
	}
	return value;
}

/**
 * @brief	Function that delivers temperature as float
 * @retval	(float) - Returns temperature in celsius degrees
 */
float readtemp(void)
{
	uint8_t Temp_byte1, Temp_byte2, sign;
	uint8_t Presence = 0;
	int16_t temp;

	Presence = DS18B20_Start();
	if(Presence)
	{
		osDelay(1);           // osDelay(1);
		DS18B20_Write(0xCC);  // skip ROM
		DS18B20_Write(0x44);  // convert t
		osDelay(800);         // osDelay(800);
	}

	Presence = DS18B20_Start();
	if(Presence)
	{
		osDelay(1);           // osDelay(1);
		DS18B20_Write(0xCC);  // skip ROM
		DS18B20_Write(0xBE);  // Read Scratch-pad
	}

	Temp_byte1 = DS18B20_Read();
	Temp_byte2 = DS18B20_Read();

	sign = Temp_byte2 & 0x80;
	temp = (Temp_byte2 << 8) + Temp_byte1;

	if(sign)
	{
		temp = ((temp ^ 0xffff) + 1) * -1;
	}

	return temp/16.0;
}
