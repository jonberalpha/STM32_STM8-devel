/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           main.c
  * @author         Berger, Ibrahim, Markovic, Meyerowitsch
  * @brief          Main program body
  * @date           30.05.2022
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "LCD_I2C.h"
#include "string.h"
#include "printf.h" // @brief include printf and sprintf

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

/* USER CODE BEGIN PV */
#define BUFSIZE 20

extern volatile char rxbuffer[BUFSIZE];
extern volatile int line_valid;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

static void BT_receiver_init(void);
static void BT_send_cmd(char *cmd, int msdelay);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  // initialize LCD-Display in 4-bit mode
  lcd_i2c_init();
  lcd_i2c_clear();
  lcd_i2c_put_curs(0, 0);
  lcd_i2c_send_string("Init Bluetooth-");
  lcd_i2c_put_curs(1, 0);
  lcd_i2c_send_string("Modules ...");

  BT_receiver_init(); // initialize Wifi click module as bluetooth receiver
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // switch on Run-Indication LED

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */

#define NUM_of_CMDs 9

/**
 * @brief	Function that initializes Wifi Click module as bluetooth receiver
 * @param   (none)
 * @retval	(none)
 */
static void BT_receiver_init(void)
{
	char receiver_cmd_list[NUM_of_CMDs][60] =
	{
		"AT+RST\r\n",                             // reset module
		"AT\r\n",                                 // check if responsive
		"AT+BLEINIT=1\r\n",                       // config BT-Module as client
		"AT+BLESCAN=1,3\r\n",                     // scan for modules
		"AT+BLECONN=0,\"84:cc:a8:47:49:86\"\r\n", // establish connection
		"AT+BLEGATTCPRIMSRV=0\r\n",               // discover services
		"AT+BLEGATTCCHAR=0,3\r\n",                // discover characteristics
		"AT+BLESPPCFG=1,3,5,3,7\r\n",             // set charachtaristic to TX/RX channel
		"AT+BLESPP\r\n"                           // enable BLE SPP (serial port passthrough)
	};

	BT_send_cmd(receiver_cmd_list[0], 2000);
	BT_send_cmd(receiver_cmd_list[1], 8000);
	BT_send_cmd(receiver_cmd_list[2], 6000);
	BT_send_cmd(receiver_cmd_list[3], 6000);
	BT_send_cmd(receiver_cmd_list[4], 8000);
	BT_send_cmd(receiver_cmd_list[5], 2000);
	BT_send_cmd(receiver_cmd_list[6], 2000);
	BT_send_cmd(receiver_cmd_list[7], 2000);
	BT_send_cmd(receiver_cmd_list[8], 2000);
}

/**
 * @brief	Function that sends given AT command to Wifi Click module
 * @param   (char *) - String of the AT command which should be sent
 * @param   (int) - Time in ms to wait after transmitted command
 * @retval	(none)
 */
static void BT_send_cmd(char *cmd, int msdelay)
{
	char rcv_buffer[256] = "";
	char tx_buffer[60] = "";

	memset((void *)rcv_buffer, 0, sizeof(rcv_buffer));
	memcpy(tx_buffer, cmd, strlen(cmd)+1);
	HAL_UART_Transmit(&huart1, (uint8_t *)tx_buffer, strlen(tx_buffer)*sizeof(char), 500);
	HAL_UART_Receive(&huart1, (uint8_t *)rcv_buffer, sizeof(rcv_buffer), 8000);
	printf("%s", rcv_buffer);
	HAL_Delay(msdelay);
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
