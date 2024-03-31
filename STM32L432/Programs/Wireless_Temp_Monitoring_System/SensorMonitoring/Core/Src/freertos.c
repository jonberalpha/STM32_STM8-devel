/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file          freertos.c
  * @authors       Berger, Ibrahim, Markovic, Meyerowitsch
  * @brief         Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "usart.h" // @brief include to use uart (huart2)
#include "LCD_I2C.h" // @brief include LCD-I2C function
#include "printf.h" // @brief include printf and sprintf
#include "stdlib.h"
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/**
 * @name Define-Extern-Group
 * Make necessary BUFSIZE-define and name extern global variables:
 */
/**@{*/
#define BUFSIZE 20

extern volatile char rxbuffer[BUFSIZE];
extern volatile int line_valid;
/**@}*/

/* USER CODE END Variables */
/* Definitions for BTRecvTask */
osThreadId_t BTRecvTaskHandle;
const osThreadAttr_t BTRecvTask_attributes = {
  .name = "BTRecvTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LCDSendTask */
osThreadId_t LCDSendTaskHandle;
const osThreadAttr_t LCDSendTask_attributes = {
  .name = "LCDSendTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for UARTSendTask */
osThreadId_t UARTSendTaskHandle;
const osThreadAttr_t UARTSendTask_attributes = {
  .name = "UARTSendTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for lcdqueue */
osMessageQueueId_t lcdqueueHandle;
const osMessageQueueAttr_t lcdqueue_attributes = {
  .name = "lcdqueue"
};
/* Definitions for uartqueue */
osMessageQueueId_t uartqueueHandle;
const osMessageQueueAttr_t uartqueue_attributes = {
  .name = "uartqueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

int _write(int file, char *ptr, int len);

/* USER CODE END FunctionPrototypes */

void BTRecvTask_do(void *argument);
void LCDSendTask_do(void *argument);
void UARTSendTask_do(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of lcdqueue */
  lcdqueueHandle = osMessageQueueNew (8, sizeof(float), &lcdqueue_attributes);

  /* creation of uartqueue */
  uartqueueHandle = osMessageQueueNew (8, sizeof(float), &uartqueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of BTRecvTask */
  BTRecvTaskHandle = osThreadNew(BTRecvTask_do, NULL, &BTRecvTask_attributes);

  /* creation of LCDSendTask */
  LCDSendTaskHandle = osThreadNew(LCDSendTask_do, NULL, &LCDSendTask_attributes);

  /* creation of UARTSendTask */
  UARTSendTaskHandle = osThreadNew(UARTSendTask_do, NULL, &UARTSendTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_BTRecvTask_do */
/**
  * @brief  Function implementing the BTRecvTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_BTRecvTask_do */
void BTRecvTask_do(void *argument)
{
  /* USER CODE BEGIN BTRecvTask_do */
  float temp = 0.0;

  /* Infinite loop */
  for(;;)
  {
	HAL_UART_Receive_IT(&huart1, (uint8_t *)rxbuffer, sizeof(rxbuffer));

	temp = (float)atof((char *)rxbuffer);
	osMessageQueuePut(lcdqueueHandle, &temp, 0, 10U); // put data in the message queue for LCD-Display Task
	osMessageQueuePut(uartqueueHandle, &temp, 0, 10U); // put data in the message queue for UART Task
	osDelay(1000);
  }
  /* USER CODE END BTRecvTask_do */
}

/* USER CODE BEGIN Header_LCDSendTask_do */
/**
* @brief Function implementing the LCDSendTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LCDSendTask_do */
void LCDSendTask_do(void *argument)
{
  /* USER CODE BEGIN LCDSendTask_do */
  osStatus_t status = osError; // initialize status as error
  float msgvalue = 0.0;
  char lcdbuf[20] = "";

  /* Infinite loop */
  for(;;)
  {
	status = osMessageQueueGet(lcdqueueHandle, &msgvalue, 0, osWaitForever); // get lasttemperaturevalue from the queue
	if(status == osOK)
	{
	                         //               Z S
	  lcd_i2c_put_curs(0,0); // set cursor to 0,0
	  sprintf(lcdbuf, "Last Temp-Value:");
	  lcd_i2c_send_string(lcdbuf);

	  lcd_i2c_put_curs(1,0); // set cursor to 1,0
	  sprintf(lcdbuf, "%3.1foC       ", msgvalue);
	  lcd_i2c_send_string(lcdbuf);
	}
    osDelay(1000);
  }
  /* USER CODE END LCDSendTask_do */
}

/* USER CODE BEGIN Header_UARTSendTask_do */
/**
* @brief Function implementing the UARTSendTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UARTSendTask_do */
void UARTSendTask_do(void *argument)
{
  /* USER CODE BEGIN UARTSendTask_do */
  osStatus_t status = osError; // initialize status as error;
  float msgvalue = 0.0;
  /* Infinite loop */
  for(;;)
  {
	status = osMessageQueueGet(uartqueueHandle, &msgvalue, 0, osWaitForever); // get lasttemperaturevalue from the queue
	if(status == osOK)
	{
	  printf("Received Temp-Value: %5.1foC\r\n", msgvalue);
	}
    osDelay(1000);
  }
  /* USER CODE END UARTSendTask_do */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/**
 * @brief	Overwrites _write-Function in syscalls.c to work with uart
 * @param	(int)file - File descriptor of file into which data is written
 * @param	(char*)ptr - Data to be written
 * @param	(int)len - Number of bytes
 * @retval	(int) - Returns the number of bytes written
 */
int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
	return len;
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
