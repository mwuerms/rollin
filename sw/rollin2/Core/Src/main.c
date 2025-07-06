/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "project.h"
#include "uart.h"
#include "str_buf.h"
#include "bldc_motor.h"
#include "bldc_driver.h"
#include "angle_sensor.h"

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
bldc_motor_t m1;
bldc_driver_t md1;
angle_sens_t as1;
uart_t u1;

#define MAIN_STR_BUF_SIZE (128)
char main_str_buf[MAIN_STR_BUF_SIZE];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void waitabit(uint32_t x) {
	while(x) x--;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	uint32_t now, last_tick, waitcnt;
	float dt, ang_deg_vel, prev_angle_deg, delta;
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
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  uart_init(&u1, USART1);

  angle_sensor_init(&as1, &hi2c1);

  bldc_driver_init(&md1, TIM1);
  bldc_driver_set_enable_pin(&md1, MOT0_EN_GPIO_Port, MOT0_EN_Pin);

  bldc_motor_init(&m1, &md1, &as1);
  bldc_motor_set_ctrl_type(&m1, BLDC_MOTOR_CTRL_TYPE_VELOCITY_OPENLOOP);
  bldc_motor_set_motor_parameters(&m1, 7, 360, 0.4f);
  bldc_motor_set_pid(&m1, 1.0f, 1.0f, 0.0f);
  bldc_motor_set_voltage_limit(&m1, 1.0f);
  bldc_motor_set_target_speed(&m1, 1.0f);
  bldc_motor_set_target_angle_deg(&m1, 135.0f);

  bldc_driver_enable(&md1);
  //bldc_driver_set_pwm(&md1, 100, 200, 300);

  bldc_motor_enable(&m1);
  m1.set.voltage_q = 0.125f;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  now = HAL_GetTick();
  last_tick = now;
  uart_send_string_blocking(&u1, "hello from the other side :-D\n");
  angle_sensor_get(&as1);
  str_buf_clear(main_str_buf, MAIN_STR_BUF_SIZE);
  str_buf_append_uint16(main_str_buf, MAIN_STR_BUF_SIZE, as1.raw_angle);
  str_buf_append_string(main_str_buf, MAIN_STR_BUF_SIZE, "\n");
  uart_send_string_blocking(&u1, main_str_buf);
  waitcnt = 1;//0000;
  while (1)
  {
	  //waitabit(500000);
	  waitabit(waitcnt);

	  now = HAL_GetTick();
	  dt = (float)(now - last_tick)/1000.0f; // in ms
	  last_tick = now;

	  bldc_motor_move(&m1, dt);
//#if 0
	  str_buf_clear(main_str_buf, MAIN_STR_BUF_SIZE);
	  str_buf_append_float(main_str_buf, MAIN_STR_BUF_SIZE, m1.current.angle_deg, 3);
	  str_buf_append_string(main_str_buf, MAIN_STR_BUF_SIZE, ",");
//	  str_buf_append_float(main_str_buf, MAIN_STR_BUF_SIZE, m1.current.angle_deg_s, 3);
//	  str_buf_append_string(main_str_buf, MAIN_STR_BUF_SIZE, ",");
//	  str_buf_append_uint16(main_str_buf, MAIN_STR_BUF_SIZE, as1.raw_angle);
//	  str_buf_append_string(main_str_buf, MAIN_STR_BUF_SIZE, ",");
	  //str_buf_append_float(main_str_buf, MAIN_STR_BUF_SIZE, ang_deg_vel, 5);*/

	  //str_buf_append_float(main_str_buf, MAIN_STR_BUF_SIZE, statangle, 5);
	  //str_buf_append_string(main_str_buf, MAIN_STR_BUF_SIZE, ",");

	  str_buf_append_float(main_str_buf, MAIN_STR_BUF_SIZE, dbg_uu, 5);
	  str_buf_append_string(main_str_buf, MAIN_STR_BUF_SIZE, ",");
	  str_buf_append_float(main_str_buf, MAIN_STR_BUF_SIZE, dbg_uv, 5);
	  str_buf_append_string(main_str_buf, MAIN_STR_BUF_SIZE, ",");
	  str_buf_append_float(main_str_buf, MAIN_STR_BUF_SIZE, dbg_uw, 5);
	  str_buf_append_string(main_str_buf, MAIN_STR_BUF_SIZE, ",");

	  str_buf_append_uint16(main_str_buf, MAIN_STR_BUF_SIZE, statangle_u);
	  str_buf_append_string(main_str_buf, MAIN_STR_BUF_SIZE, ",");
	  str_buf_append_uint16(main_str_buf, MAIN_STR_BUF_SIZE, statangle_v);
	  str_buf_append_string(main_str_buf, MAIN_STR_BUF_SIZE, ",");
	  str_buf_append_uint16(main_str_buf, MAIN_STR_BUF_SIZE, statangle_w);
	  str_buf_append_string(main_str_buf, MAIN_STR_BUF_SIZE, ",");
	  str_buf_append_uint16(main_str_buf, MAIN_STR_BUF_SIZE, statangle_inc);

	  str_buf_append_string(main_str_buf, MAIN_STR_BUF_SIZE, "\n");
	  uart_send_string_blocking(&u1, main_str_buf);
//#endif
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_8);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(64000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
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
