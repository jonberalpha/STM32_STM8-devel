/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file              freertos.c
  * @authors           Berger, Ibrahim, Markovic, Meyerowitsch
  * @brief             Code for freertos applications
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

#include "ds18b20.h" // @brief include readtemp()-function
#include "usart.h"  // @brief include huart1
#include "printf.h" // @brief include printf and sprintf
#include "string.h" // @brief include strlen

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
/* USER CODE END Variables */
/* Definitions for BTSendTask */
osThreadId_t BTSendTaskHandle;
const osThreadAttr_t BTSendTask_attributes = {
  .name = "BTSendTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for GetTempTask */
osThreadId_t GetTempTaskHandle;
const osThreadAttr_t GetTempTask_attributes = {
  .name = "GetTempTask",
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
/* Definitions for btqueue */
osMessageQueueId_t btqueueHandle;
const osMessageQueueAttr_t btqueue_attributes = {
  .name = "btqueue"
};
/* Definitions for uartqueue */
osMessageQueueId_t uartqueueHandle;
const osMessageQueueAttr_t uartqueue_attributes = {
  .name = "uartqueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void BTSendTask_do(void *argument);
void GetTempTask_do(void *argument);
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
  /* creation of btqueue */
  btqueueHandle = osMessageQueueNew (8, sizeof(float), &btqueue_attributes);

  /* creation of uartqueue */
  uartqueueHandle = osMessageQueueNew (8, sizeof(float), &uartqueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of BTSendTask */
  BTSendTaskHandle = osThreadNew(BTSendTask_do, NULL, &BTSendTask_attributes);

  /* creation of GetTempTask */
  GetTempTaskHandle = osThreadNew(GetTempTask_do, NULL, &GetTempTask_attributes);

  /* creation of UARTSendTask */
  UARTSendTaskHandle = osThreadNew(UARTSendTask_do, NULL, &UARTSendTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_BTSendTask_do */
/**
  * @brief  Function implementing the BTSendTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_BTSendTask_do */
void BTSendTask_do(void *argument)
{
  /* USER CODE BEGIN BTSendTask_do */
  osStatus_t status = osError; // initialize status as error
  float msgvalue = 0.0;
  char buffer[20] = "";
  /* Infinite loop */
  for(;;)
  {
	status = osMessageQueueGet(btqueueHandle, &msgvalue, 0, osWaitForever); // get lasttemperaturevalue from the queue
	if(status == osOK)
	{
		snprintf(buffer, sizeof(buffer), "%f\r\n", msgvalue); // convert float to string
		HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer)*sizeof(char), 10U); // transmit temperaturevalue over BT-module
	}
    osDelay(1000);
  }
  /* USER CODE END BTSendTask_do */
}

/* USER CODE BEGIN Header_GetTempTask_do */
/**
* @brief Function implementing the GetTempTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_GetTempTask_do */
void GetTempTask_do(void *argument)
{
  /* USER CODE BEGIN GetTempTask_do */
  float temp = 0.0;
  /* Infinite loop */
  for(;;)
  {
	temp = readtemp(); // read temperature of 1-wire temperature sensor
	osMessageQueuePut(btqueueHandle, &temp, 0, 10U); // put data in the message queue for BT Task
	osMessageQueuePut(uartqueueHandle, &temp, 0, 10U); // put data in the message queue for UART Task
	osDelay(1000);
  }
  /* USER CODE END GetTempTask_do */
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
	  printf("Sent Temp-Value: %5.1foC\r\n", msgvalue); // log temp-values over UART2/USB
	}
	osDelay(1000);
  }
  /* USER CODE END UARTSendTask_do */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
