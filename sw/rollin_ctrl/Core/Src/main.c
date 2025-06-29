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
volatile uint32_t global_event = 0;

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
	angle_deg = 0.80f * (angle_deg_prev + gyr_z_angle_deg) + 0.20f * acc_xy_angle_deg;
	angle_deg_prev = angle_deg;
}

static void read_mpu9250_string(void) {
	read_mpu9250_values();

	// using Tauno, send as string using "," as separator: value1,value2,value3
	str_buf_clear(mpu_str, MPU_STR_SIZE);
	str_buf_append_float(mpu_str, MPU_STR_SIZE, acc_xy_angle_deg, 3);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, ",");
	str_buf_append_float(mpu_str, MPU_STR_SIZE, gyr_z_angle_deg, 3);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, ",");
	str_buf_append_float(mpu_str, MPU_STR_SIZE, angle_deg, 3);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, "\n");
	uart_send_string(mpu_str);
	/*
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
	*/
}

uint16_t get_program_id_string(char *str, uint16_t str_size) {
	str_buf_clear(mpu_str, MPU_STR_SIZE);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, "program id: ");
	str_buf_append_string(mpu_str, MPU_STR_SIZE, PROGRAM_NAME);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, ", compiled: ");
	str_buf_append_string(mpu_str, MPU_STR_SIZE, __DATE__);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, ", ");
	str_buf_append_string(mpu_str, MPU_STR_SIZE, __TIME__);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	uint32_t primask;
	volatile uint16_t adc_value;
	volatile int32_t speed;
	uint32_t local_event;
	uint16_t local_cnt;
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
	foc_set_speed(FOC_MOTOR1, 1000);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_12|LL_GPIO_PIN_13);

	get_program_id_string(mpu_str, MPU_STR_SIZE);
	uart_send_string(mpu_str);
	_wait_some_time();

	str_buf_clear(mpu_str, MPU_STR_SIZE);
	str_buf_append_string(mpu_str, MPU_STR_SIZE, "\nusing Tauno-serial-plotter\n");
	uart_send_string(mpu_str);
	_wait_some_time();

	local_event = 0;
	local_cnt = 0;
	while(1) {
		/*if(local_event & 1) {
			// FOC
			local_cnt++;
			str_buf_clear(mpu_str, MPU_STR_SIZE);
			//str_buf_append_uint16(mpu_str, MPU_STR_SIZE, local_cnt);
			//str_buf_append_string(mpu_str, MPU_STR_SIZE, ",");
			//str_buf_append_uint16(mpu_str, MPU_STR_SIZE, motors[FOC_MOTOR1].x);
			//str_buf_append_string(mpu_str, MPU_STR_SIZE, ",");
			str_buf_append_uint16(mpu_str, MPU_STR_SIZE, sin_lut[motors[FOC_MOTOR1].sin.u]);
			str_buf_append_string(mpu_str, MPU_STR_SIZE, ",");
			str_buf_append_uint16(mpu_str, MPU_STR_SIZE, sin_lut[motors[FOC_MOTOR1].sin.v]);
			str_buf_append_string(mpu_str, MPU_STR_SIZE, ",");
			str_buf_append_uint16(mpu_str, MPU_STR_SIZE, sin_lut[motors[FOC_MOTOR1].sin.w]);
			str_buf_append_string(mpu_str, MPU_STR_SIZE, "\n");
			uart_send_string(mpu_str);
		}*/
		if(local_event & 0x0002) {
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
			adc_value = HAL_ADC_GetValue(&hadc1);

			if(adc_value > 2148) {
				speed = (int16_t)(((adc_value - 1948) * 1000)/1948 + 0);
			}
			else if(adc_value < 1948) {
				speed = (int16_t)((adc_value * 1000)/1948 -1000);
			}
			else {
				speed = 0;
			}
			foc_set_speed(FOC_MOTOR1, speed);
		}
		// wait for global events
		while(1) {
			// sync global events
			primask = __get_PRIMASK();  // Save current interrupt state
			__disable_irq();            // Disable all interrupts

			local_event = global_event;
			global_event = 0;

			__set_PRIMASK(primask);     // Restore previous interrupt state

			if(local_event) {
				// process event
				break;
			}

			// else go to sleep
			LL_LPM_EnableSleep();  // This clears SLEEPDEEP

			// Enter sleep mode (Wait For Interrupt)
			__WFI();  // or __WFE();

		}
	}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		//read_mpu9250_string();
		//LL_ADC_REG_StartConversionSWStart(ADC1);
		_wait_some_time();
		//adc_value = adc1_read();
		//adc_value = LL_ADC_REG_ReadConversionData12(ADC1);


/*
		str_buf_clear(mpu_str, MPU_STR_SIZE);
		str_buf_append_uint16(mpu_str, MPU_STR_SIZE, adc_value);
		str_buf_append_string(mpu_str, MPU_STR_SIZE, "\n");
		uart_send_string(mpu_str);*/



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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_SetSystemCoreClock(8000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_2);
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
