/**
 * Martin Egli
 * 2025-05-14
 * 
 * drive a BLDC motor using FOC
 * 
 * FOC: Field Oriented Control
 *   see: https://en.wikipedia.org/wiki/Vector_control_(motor)
 *        https://www.pmdcorp.com/resources/type/articles/get/field-oriented-control-foc-a-deep-dive-article
 *        and many more
 * this module is mcu dependend, supported: STM32F103
 */

#ifndef _FOC_H_
#define _FOC_H_

// - check for mcu -------------------------------------------------------------
#ifdef STM32F103xB
	#warning FOC: MCU STM32F103xB is supported
#else
	#error FOC: this MCU is not supported
#endif

// - include -------------------------------------------------------------------
#include "stdint.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_tim.h"

#define FOC_NB_BLDC_MOTORS (2)
#define FOC_MOTOR1 (0)
#define FOC_MOTOR2 (1)

typedef struct {
	TIM_TypeDef *TIM;
	struct {
		uint16_t u, v, w; // indexes for the phases u, v, w
	} sin;
	struct {
		uint16_t speed;
		uint16_t dir;
		uint16_t tim_ccr;
		uint16_t tim_channel;
	} speed_ctrl;
	uint16_t x;
} foc_t;

// - public functions ----------------------------------------------------------

/**
 * initialize all hw and vars
 * @return number of motors
 */
uint16_t foc_init(void);
void foc_start(void);
//void foc_enable(uint16_t motor);
//void foc_disable(uint16_t motor);
void foc_set_speed(uint16_t mi, int16_t speed);
void foc_ctrl_process(void);

#endif // _FOC_H_
