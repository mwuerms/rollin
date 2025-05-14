/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : main.c
* @brief          : Main program body
******************************************************************************
* @attention
*
* Copyright (c) 2024 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mpu9250.h"
#include "uart.h"
#include "str_buf.h"
#include "foc.h"

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define MPU_STR_SIZE (128)
static char mpu_str[MPU_STR_SIZE];

void _wait_some_time(void) {
	for(uint32_t cnt = 50000; cnt; cnt--) {
		asm("NOP");
	}
}

static float acc_xy_angle_deg;
static float gyr_z_angle_deg_rate, gyr_z_angle_deg;
static float angle_deg, angle_deg_prev = 0;

float get_angle_deg(void) {
	return angle_deg;
}

void read_mpu9250_values(void) {
	mpu9250_read_sensor_values();
	acc_xy_angle_deg = mpu9250_get_acc_xy_angle_deg();
	acc_xy_angle_deg -= 90.0f;
	gyr_z_angle_deg_rate = mpu9250_get_gyr_z_angle_rate_deg_pro_s();
	gyr_z_angle_deg = gyr_z_angle_deg_rate * 0.01201;//0.01192; //meas_time in s

	// complementary filter for angle from accelerometer and gyroscope
	angle_deg = 0.95 * (angle_deg_prev + gyr_z_angle_deg) + 0.05 * acc_xy_angle_deg;
	angle_deg_prev = angle_deg;
}

static void read_mpu9250_string(void) {
	read_mpu9250_values();

	// using https://github.com/qwertpas/SerpentSerialTool
	str_buf_clear(mpu_str, MPU_STR_SIZE);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, "acc_xy_deg: ");
	str_buf_append_float(mpu_str, MPU_STR_SIZE, acc_xy_angle_deg, 3);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, "\n");
	uart_send_string(mpu_str);

	str_buf_clear(mpu_str, MPU_STR_SIZE);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, "gyr_z_deg: ");
	str_buf_append_float(mpu_str, MPU_STR_SIZE, gyr_z_angle_deg, 3);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, "\n");
	uart_send_string(mpu_str);

	str_buf_clear(mpu_str, MPU_STR_SIZE);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, "angle_deg: ");
	str_buf_append_float(mpu_str, MPU_STR_SIZE, angle_deg, 3);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, "\n\t");
	uart_send_string(mpu_str);
}
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
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	uart_init();
	mpu9250_init();

	foc_init();
	foc_start();

	str_buf_clear(mpu_str, MPU_STR_SIZE);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, "testing 64 bytes, not so much\n");
	uart_send_string(mpu_str);
	_wait_some_time();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		//read_mpu9250_string();
		_wait_some_time();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
