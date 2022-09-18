/**
 * Martin Egli
 * 2021-09-16
 *
 * mpu6500 module
 */
#ifndef _MPU6500_H_
#define _MPU6500_H_

// - includes ------------------------------------------------------------------
#include <stdint.h>

// - functions -----------------------------------------------------------------
uint8_t mpu6500_init(void);
uint8_t mpu6500_set_config(void);
uint8_t mpu6500_read_sensor_data(uint8_t *buffer, uint8_t length);

#endif // _MPU6500_H_