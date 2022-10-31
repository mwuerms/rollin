/*
 * as5600.c
 *
 *  Created on: Oct 26, 2022
 *      Author: martin
 */

// - include -------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

// - i2c definitions -----------------------------------------------------------
#define AS5600_I2C			I2C1
#define AS5600_I2C_ADDR 	((0x36) << 1)
#define AS5600_I2C_WRITE	(AS5600_I2C_ADDR | 0)
#define AS5600_I2C_READ		(AS5600_I2C_ADDR | 1)

// - private functions ---------------------------------------------------------
static inline void i2c_start_condition(void) {
	LL_I2C_GenerateStartCondition(AS5600_I2C);
	LL_I2C_IsActiveFlag_BUSY(AS5600_I2C);
}

static inline void i2c_stop_condition(void) {
	LL_I2C_GenerateStopCondition(AS5600_I2C);
	LL_I2C_IsActiveFlag_BUSY(AS5600_I2C);
}

// ack: LL_I2C_ACK, LL_I2C_NACK
static uint8_t i2c_transmit(uint8_t byte, uint32_t ack) {
	LL_I2C_AcknowledgeNextData(AS5600_I2C, ack);
	LL_I2C_TransmitData8(AS5600_I2C, byte);
	while(LL_I2C_IsActiveFlag_BTF(AS5600_I2C) == false);
	//while(LL_I2C_IsActiveFlag_RXNE(AS5600_I2C) == false);
	byte = LL_I2C_ReceiveData8(AS5600_I2C);
	return byte;
}

static uint16_t i2c_write(uint8_t reg_addr, uint8_t *wr_data, uint16_t wr_size) {
	uint16_t n;
	i2c_start_condition();

	i2c_transmit(AS5600_I2C_WRITE, LL_I2C_NACK);
	i2c_transmit(reg_addr, LL_I2C_NACK);

	for(n = 0; n != wr_size; n++) {
		i2c_transmit(wr_data[n], LL_I2C_NACK);
		// get ACK
	}

	i2c_stop_condition();
	return n;
}

static uint16_t i2c_read(uint8_t reg_addr, uint8_t *rd_data, uint16_t rd_size) {
	uint16_t n;
	i2c_write(reg_addr, NULL, 0);
	i2c_start_condition();

	i2c_transmit(AS5600_I2C_READ, LL_I2C_NACK);
	i2c_transmit(reg_addr, LL_I2C_NACK);

	for(n = 0; n != rd_size; n++) {
		rd_data[n] = i2c_transmit(0xFF, LL_I2C_ACK);
		// send ACK
	}
	i2c_stop_condition();
	return n;
}

// - public functions ----------------------------------------------------------
void as5600_init(void) {
	// copied from STM32Cube Init
	LL_I2C_InitTypeDef I2C_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
	/**I2C1 GPIO Configuration
	PB6   ------> I2C1_SCL
	PB7   ------> I2C1_SDA
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	// Peripheral clock enable
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

	LL_I2C_DisableOwnAddress2(I2C1);
	LL_I2C_DisableGeneralCall(I2C1);
	LL_I2C_EnableClockStretching(I2C1);
	I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
	I2C_InitStruct.ClockSpeed = 100000;
	I2C_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_2;
	I2C_InitStruct.OwnAddress1 = 0;
	I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
	I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
	LL_I2C_Init(I2C1, &I2C_InitStruct);
	LL_I2C_SetOwnAddress2(I2C1, 0);
}
/*	LL_I2C_InitTypeDef i2c_init;
	i2c_init.ClockSpeed = LL_I2C_CLOCK_SPEED_FAST_MODE;
	i2c_init.DutyCycle = LL_I2C_DUTYCYCLE_2;
	i2c_init.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
	i2c_init.OwnAddress1 = 0xAB;
	i2c_init.PeripheralMode = LL_I2C_MODE_I2C;
	i2c_init.TypeAcknowledge = I2C_CR1_ACK;

	LL_I2C_Init(AS5600_I2C, &i2c_init);
	LL_I2C_Enable(AS5600_I2C);
}*/

void as5600_set_config(uint16_t conf) {
	uint8_t buf[32];
	i2c_read(0x00, buf, 9);
	i2c_read(0x0B, buf, 5);

}

void as5600_set_zeropos_maxpos(uint16_t zpos, uint16_t mpos) {
	zpos &= 0x0FFF;
	mpos &= 0x0FFF;
}

uint16_t as5600_read_raw_angle(void) {
	return 0;
}

uint16_t as5600_read_angle(void) {
	return 0;
}

uint8_t as5600_read_status(void) {
	return 0;
}
