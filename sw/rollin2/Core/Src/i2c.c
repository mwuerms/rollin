/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */
  //LL_I2C_Disable(I2C1);
  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}
/* I2C2 init function */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */
  //LL_I2C_Disable(I2C2);
  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_I2C1_ENABLE();

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
  else if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
  else if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();

    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/*
uint16_t i2c_wr_blocking(I2C_TypeDef *hi2c, uint8_t addr, uint8_t *buf, uint16_t buf_len) {
	uint16_t n;
    // tx i2c_addr + WR
    if(hi2c == NULL) {
        // error, invalid sensor
        return 0;
    }

    LL_I2C_Enable(hi2c);
    / *if(LL_I2C_IsEnabled(hi2c) == 0) {
    	LL_I2C_Enable(hi2c);
    }* /

    // Warten bis Bus frei
	if(LL_I2C_IsActiveFlag_BUSY(I2C1)) {
		LL_I2C_Enable(hi2c);
	}
	if(LL_I2C_IsActiveFlag_BUSY(I2C1) == 0) {
		LL_I2C_Enable(hi2c);
	}

    LL_I2C_GenerateStartCondition(hi2c);
    while(LL_I2C_IsActiveFlag_SB(hi2c) == 0); // wait for start done

	LL_I2C_TransmitData8(hi2c, (addr << 1) | 0); // 0: write
	while(LL_I2C_IsActiveFlag_ADDR(hi2c) == 0); // wait for addr
	LL_I2C_ClearFlag_ADDR(hi2c);
	// check for N/ACK

	for(n = 0; n < buf_len; n++) {
		LL_I2C_TransmitData8(hi2c, buf[n]);
		while(LL_I2C_IsActiveFlag_TXE(hi2c) == 0); // wait until data is sent
		// check for N/ACK
	}

    LL_I2C_GenerateStopCondition(hi2c);
    return n;
}

uint16_t i2c_rd_blocking(I2C_TypeDef *hi2c, uint8_t addr, uint8_t *buf, uint16_t buf_len) {
	uint16_t n;
    // tx i2c_addr + RD
    if(hi2c == NULL) {
        // error, invalid sensor
        return 0;
    }

    LL_I2C_Enable(hi2c);
    / *if(LL_I2C_IsEnabled(hi2c) == 0) {
    	LL_I2C_Enable(hi2c);
    }* /


    LL_I2C_GenerateStartCondition(hi2c);
    while(LL_I2C_IsActiveFlag_SB(hi2c) == 0); // wait for start done

	LL_I2C_TransmitData8(hi2c, (addr << 1) | 1); // 1: read
	while(LL_I2C_IsActiveFlag_ADDR(hi2c) == 0); // wait for addr
	LL_I2C_ClearFlag_ADDR(hi2c);
	// check for N/ACK

	for(n = 0; n < buf_len; n++) {
		LL_I2C_TransmitData8(hi2c, 0xFF);
		while(LL_I2C_IsActiveFlag_TXE(hi2c) == 0); // wait until data is sent
		// check for N/ACK
		buf[n] = LL_I2C_ReceiveData8(hi2c);
		// last byte: gen NACK
	}

    LL_I2C_GenerateStopCondition(hi2c);

    return 0;
}
*/

/* USER CODE END 1 */
