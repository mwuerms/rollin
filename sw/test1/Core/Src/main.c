/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static volatile uint16_t sine_lookup_table[] = {32767, 37043, 41247, 45306, 49150, 52714, 55936, 58762, 61144, 63039, 64417, 65253, 65534, 65253, 64417, 63039, 61144, 58762, 55936, 52714, 49150, 45306, 41247, 37043, 32767, 28490, 24286, 20227, 16383, 12819, 9597, 6771, 4389, 2494, 1116, 280, 0, 280, 1116, 2494, 4389, 6771, 9597, 12819, 16383, 20227, 24286, 28490};
static const volatile uint16_t sine_lookup_table_size = sizeof(sine_lookup_table)/sizeof(sine_lookup_table[0]);
static volatile uint16_t pwm_A_index, pwm_B_index, pwm_C_index;
static uint16_t _inc(uint16_t x, uint16_t max) {
	if(x < max) {
		return x+1;
	}
	return 0;
}
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if(htim == &htim1) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
			pwm_A_index = _inc(pwm_A_index, sine_lookup_table_size-1);
			TIM1->CCR1 = sine_lookup_table[pwm_A_index];
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
			pwm_B_index = _inc(pwm_B_index, sine_lookup_table_size-1);
			TIM1->CCR2 = sine_lookup_table[pwm_B_index];
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
			pwm_C_index = _inc(pwm_C_index, sine_lookup_table_size-1);
			TIM1->CCR3 = sine_lookup_table[pwm_C_index];
		}
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	asm("NOP");
}

#define COM_BUFFER_SIZE	(256)
static struct {
	uint8_t data[COM_BUFFER_SIZE];
	uint16_t wr_idx;
} com_buffer;

void com_clear_buffer(void) {
	com_buffer.wr_idx = 0;
}

#include "usbd_cdc_if.h"
void com_send_buffer(void) {
	if(com_buffer.wr_idx == 0) {
		// nothing to send
		return;
	}
	if(CDC_Transmit_FS(com_buffer.data, com_buffer.wr_idx) == 0) {
		// OK
		asm("NOP");
	}
}


uint32_t com_append_char(char c) {
	if(com_buffer.wr_idx < (COM_BUFFER_SIZE-2)) {
		com_buffer.data[com_buffer.wr_idx++] = c;
		return 1;
	}
	return 0;
}

void com_append_string(char *str) {
	char c;
	// copy string
	while(1) {
		c = *str++;
		if(c == '\0') {
			// found end, do not copy
			break;
		}
		if(com_append_char(c) == 0) {
			// buffer full, stop
			break;
		}
	}
}

void com_append_int16(int16_t value) {
	uint16_t digit_pos, use_char;
	int16_t digits[] = {10000, 1000, 100, 10, 1};
	uint8_t digit_value;
	use_char = 0;
	// sign
	if(value < 0) {
		value *= -1;
		if(com_append_char('-') == 0) {
			// buffer full, stop
			return;
		}
	}
	if(value == 0) {
		// only append '0', and done here
		com_append_char('0');
		return;
	}
	for(digit_pos = 0; digit_pos < (sizeof(digits)/sizeof(digits[0])); digit_pos++) {
		digit_value = (uint8_t)(value/digits[digit_pos]);
		if((digit_value != 0) || (use_char != 0)) {
			use_char = 1;
			if(com_append_char('0'+digit_value) == 0) {
				// buffer full, stop
				return;
			}
			value -= digit_value * digits[digit_pos];
		}
	}
}

void com_append_int32(int32_t value) {
	uint16_t digit_pos, use_char;
	int32_t digits[] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
	uint8_t digit_value;
	use_char = 0;
	// sign
	if(value < 0) {
		value *= -1;
		if(com_append_char('-') == 0) {
			// buffer full, stop
			return;
		}
	}
	if(value == 0) {
		// only append '0', and done here
		com_append_char('0');
		return;
	}
	for(digit_pos = 0; digit_pos < (sizeof(digits)/sizeof(digits[0])); digit_pos++) {
		digit_value = (uint8_t)(value/digits[digit_pos]);
		if((digit_value != 0) || (use_char != 0)) {
			use_char = 1;
			if(com_append_char('0'+digit_value) == 0) {
				// buffer full, stop
				return;
			}
			value -= digit_value * digits[digit_pos];
		}
	}
}

void com_append_float(float f, uint32_t nb_dec_places) {
	//int32_t digits_reverse[] = {1, 10, 100, 1000, 10000, 100000, 1000000,10000000, 100000000, 1000000000};
	int32_t x;
	uint8_t digit;

	x = (int32_t)f;
	com_append_int32(x);

	if(nb_dec_places == 0) {
		// do not add any decimal places
		return;

	}
	if(nb_dec_places >= 9) {
		// max is 9
		nb_dec_places = 9;
	}

	f -= (float)x;
	if(f < 0.0f) {
		f *= -1;
	}
	com_append_char('.');
	// eval nb leading '0' of part after '.'
	for(uint16_t n = 0; n < nb_dec_places; n++) {
		f *= 10;
		x = (uint8_t)f;
		f -= (float)x;
		com_append_char('0'+x);
	}
}

void _wait_some_time(void) {
	for(uint32_t cnt = 1000000; cnt; cnt--) {
		asm("NOP");
	}
}

#define READ_CMD  (0x80)
#define WRITE_CMD (0x00)
void mpu9250_read_who_am_i(void) {
  uint8_t tx_data[1+1] = {READ_CMD|117};
  uint8_t rx_data[sizeof(tx_data)];
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, sizeof(tx_data), 10000);
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
}

void mpu9250_read_config(void) {
  uint8_t tx_data[1+7] = {READ_CMD|26};
  uint8_t rx_data[sizeof(tx_data)];
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, sizeof(tx_data), 10000);
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
}

void mpu9250_write_config(void) {
  uint8_t tx_data[] = {WRITE_CMD|26, 0x00, 0x00, (0x03<<2), 0x00, 0x00, 0x00};
  uint8_t rx_data[sizeof(tx_data)];
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, sizeof(tx_data), 10000);
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
}

void mpu9250_read_int_config(void) {
  uint8_t tx_data[1+4] = {READ_CMD|55};
  uint8_t rx_data[sizeof(tx_data)];
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, sizeof(tx_data), 10000);
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
}

void mpu9250_write_int_config(void) {
  uint8_t tx_data[] = {WRITE_CMD|55, 0xF0, 0x00, 0x00, 0x00};
  uint8_t rx_data[sizeof(tx_data)];
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, sizeof(tx_data), 10000);
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
}

uint16_t mpu9250_read_sensor_data(uint8_t *rx_data) {
  uint8_t tx_data[1+3*2+1*2+3*2] = {READ_CMD|59};
  //uint8_t rx_data[sizeof(tx_data)];
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, sizeof(tx_data), 10000);
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
  return sizeof(tx_data);
}

uint16_t mpu9250_read_acc_data(uint8_t *rx_data) {
  uint8_t tx_data[1+3*2] = {READ_CMD|59};
  //uint8_t rx_data[sizeof(tx_data)];
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, sizeof(tx_data), 10000);
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
  return sizeof(tx_data);
}

uint16_t mpu9250_read_temp_data(uint8_t *rx_data) {
  uint8_t tx_data[1+2] = {READ_CMD|65};
  //uint8_t rx_data[sizeof(tx_data)];
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, sizeof(tx_data), 10000);
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
  return sizeof(tx_data);
}

uint16_t mpu9250_read_gyro_data(uint8_t *rx_data) {
  uint8_t tx_data[1+3*2] = {READ_CMD|67};
  //uint8_t rx_data[sizeof(tx_data)];
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, sizeof(tx_data), 10000);
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
  return sizeof(tx_data);
}

/* MPU9250
1g = 16384 (2^14)
Accelerometer Umschlagsmessung Mittelwerte
2022-09-09, Martin Egli
angle	alpha	acc x	acc y	acc z
0	90	-116.95652173913	16533.1304347826	2896.86956521739
90	0	16381.9047619048	319.619047619048	2813.42857142857
180	-90	259	-16190.7777777778	2529.44444444444
270	-180	-16359.6470588235	-9.76470588235294	5139.17647058824

Gyro, Mittelwerte in Ruhe
gyro x	gyro y	gyro z
-249.254681220314	-99.6965046888321	-266.373261985954
 */
const float acc_1g = 16384.0f;
const float acc_meas_x_1g[] = {16381.9047619048f, -16359.6470588235f};
const float acc_meas_y_1g[] = {16533.1304347826f, -16190.7777777778f};

typedef struct {
	float gain, offset;
} gain_offset_t;

struct {
	gain_offset_t x;
	gain_offset_t y;
} mpu9250_corr_acc;
float alpha_deg_corr = -90.0f;

float mpu9250_calc_acc(int16_t value, gain_offset_t *corr) {
	return corr->gain * (float)value + corr->offset;
}
#include <math.h>
float acc_to_alpha_rad(float x, float y) {
	return atan2((double)x, (double)y);
}
float rad_to_deg(float a_rad) {
	return a_rad * 180.0f / M_PI;
}

struct {
	float x, y;
} acc_values;
float alpha, alpha_deg;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	// calc correction values for acc_x, acc_y
	mpu9250_corr_acc.x.gain    = (2*acc_1g)/(acc_meas_x_1g[0] - acc_meas_x_1g[1]); // a = dy/dx
	mpu9250_corr_acc.x.offset = acc_1g - mpu9250_corr_acc.x.gain*acc_meas_x_1g[0]; // b = y0 - a*x0

	mpu9250_corr_acc.y.gain    = (2*acc_1g)/(acc_meas_y_1g[0] - acc_meas_y_1g[1]); // a = dy/dx
	mpu9250_corr_acc.y.offset = acc_1g - mpu9250_corr_acc.y.gain*acc_meas_y_1g[0]; // b = y0 - a*x0

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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USB_DEVICE_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */


  mpu9250_read_who_am_i();
  mpu9250_read_config();
  mpu9250_write_config();
  mpu9250_read_int_config();


  asm("NOP");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t sensor_data[32];
  int16_t value;
  com_clear_buffer();
  com_append_string("acc x, acc y, acc z, gyro x, gyro y, gyro z, temp\n");
  com_send_buffer();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  com_clear_buffer();
	  mpu9250_read_sensor_data(sensor_data);
	  // acc
	  value = (int16_t)((sensor_data[1] << 8) + sensor_data[2]); // acc x
	  acc_values.x = mpu9250_calc_acc(value, &mpu9250_corr_acc.x);
	  //com_append_int16(value);
	  //com_append_char(',');
	  value = (int16_t)((sensor_data[3] << 8) + sensor_data[4]); // acc y
	  acc_values.y = mpu9250_calc_acc(value, &mpu9250_corr_acc.y);
	  //com_append_int16(value);
	  //com_append_char(',');
	  value = (int16_t)((sensor_data[5] << 8) + sensor_data[6]); // acc z
	  //com_append_int16(value);
	  //com_append_char(',');

	  // gyro
	  value = (int16_t)((sensor_data[9] << 8) + sensor_data[10]); // gyro x
	  //com_append_int16(value);
	  //com_append_char(',');
	  value = (int16_t)((sensor_data[11] << 8) + sensor_data[12]); // gyro y
	  //com_append_int16(value);
	  //com_append_char(',');
	  value = (int16_t)((sensor_data[13] << 8) + sensor_data[14]); // gyro z
	  //com_append_int16(value);
	  //com_append_char(',');

	  //com_append_float(acc_values.x, 5);
	  //com_append_char(',');

	  //com_append_float(acc_values.y, 5);
	  //com_append_char(',');


		alpha = acc_to_alpha_rad(acc_values.x, acc_values.y);
		alpha_deg = rad_to_deg(alpha);
		alpha_deg += alpha_deg_corr;

		com_append_float(alpha_deg, 5);
		com_append_char(',');

	  // temp
	  value = (int16_t)((sensor_data[7] << 8) + sensor_data[8]); // temp
	  com_append_int16(value);
	  com_append_char('\n');

	  com_send_buffer();
	  _wait_some_time();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64; // SPI_BAUDRATEPRESCALER_8; // -> 3 MHz
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
  HAL_NVIC_SetPriority(TIM1_UP_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
  //HAL_TIM_RegisterCallback(&htim1, HAL_TIM_PWM_PULSE_FINISHED_HALF_CB_ID, pwm_update_callback);
  pwm_A_index = 0;
  pwm_B_index = 16;
  pwm_C_index = 32;
  TIM1->CCR1 = sine_lookup_table[pwm_A_index];
  TIM1->CCR2 = sine_lookup_table[pwm_B_index];
  TIM1->CCR3 = sine_lookup_table[pwm_C_index];
  HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_3);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_NSS_Pin PA15 */
  GPIO_InitStruct.Pin = SPI1_NSS_Pin|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
