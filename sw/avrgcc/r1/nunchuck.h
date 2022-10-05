/**
 * Martin Egli
 * 2022-09-26
 * Nunchuck module, using i2c
 * see https://create.arduino.cc/projecthub/infusion/using-a-wii-nunchuk-with-arduino-597254
 */

#ifndef _NUNCHUCK_H_
#define _NUNCHUCK_H_

// - includes ------------------------------------------------------------------
#include <stdint.h>

// - defines -------------------------------------------------------------------

// - public functions ----------------------------------------------------------
void nunchuck_start_unencrypred(void);
uint8_t nunchuck_read_raw(uint8_t *buffer, uint8_t length);

#endif // _NUNCHUCK_H_