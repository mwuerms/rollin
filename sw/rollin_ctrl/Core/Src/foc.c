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

// - includes ------------------------------------------------------------------
#include "foc.h"
#include "math.h"
#include "main.h"
#include <stdbool.h>

// - defines -------------------------------------------------------------------
#define FOC_BLDC_MOTOR1_TIMER (TIM1)
#define FOC_BLDC_MOTOR2_TIMER (TIM2)
// specify the bldc motors
#define FOC_BLDC_MOTOR_NB_COILS (3) // 3 coils 120° appart
// phase u, v, w
#define FOC_BLDC_MOTOR_NB_POLES (12)
#define FOC_BLDC_MOTOR_NB_MAGNETS (14)

// timer resolution 16 bits
#define FOC_PWM_TIMER_FREQ (8000000UL)
//#define FOC_PWM_MAX (65535UL)
#define FOC_PWM_MAX (799UL)
#define FOC_SIN_LUT_NB_VALUES (16UL)
#define FOC_SIN_LUT_SIZE (FOC_BLDC_MOTOR_NB_COILS*FOC_SIN_LUT_NB_VALUES)
#define FOC_SIN_LUT_MAX_INDEX (FOC_SIN_LUT_SIZE - 1)
#define FOC_SIN_LUT_U_START (0 * FOC_SIN_LUT_NB_VALUES)
#define FOC_SIN_LUT_V_START (1 * FOC_SIN_LUT_NB_VALUES)
#define FOC_SIN_LUT_W_START (2 * FOC_SIN_LUT_NB_VALUES)
#define FOC_SIN_OFFSET ((FOC_PWM_MAX+1)/2) // 1/2 of max
#define FOC_SIN_AMPLITUDE ((FOC_PWM_MAX/2)/2) // this determines the current through the coils

#define FOC_NB_STEPS_PER_FULL_ROTATION (FOC_SIN_LUT_SIZE * FOC_BLDC_MOTOR_NB_POLES / FOC_BLDC_MOTOR_NB_COILS)

#define FOC_MOTOR_MAX_SPEED (1000) // in 0.01 rev/s, -> 10 rev/s = max
#define FOC_MOTOR_DIR_FOREWARD (true)
#define FOC_MOTOR_DIR_BACKWARD (false)
#define FOC_SPEED_TIM (TIM3)
#define FOC_SPEED_TIM_FREQ (8000000UL/50)

// - private variables ---------------------------------------------------------
// testing NO static
static uint16_t sin_lut[FOC_SIN_LUT_SIZE];
static foc_t motors[FOC_NB_BLDC_MOTORS];

// - private functions ---------------------------------------------------------
// increment i+1 so it keeps between 0 ... MAX, else wrap around
static inline uint16_t inc_lut_index(uint16_t i) {
	if(i == FOC_SIN_LUT_MAX_INDEX) {
		return 0;
	}
	return i+1;
}

// increment i-1 so it keeps between 0 ... MAX, else wrap around
static inline uint16_t dec_lut_index(uint16_t i) {
	if(i == 0) {
		return FOC_SIN_LUT_MAX_INDEX;
	}
	return i-1;
}

// define if increment is foreward and decrement is backward or vice versa
#define NEXT_INDEX_FOREWARD(i) inc_lut_index(i)
#define NEXT_INDEX_BACKWARD(i) dec_lut_index(i)

#define IS_MOTOR_INDEX_VALID(mi) (mi < FOC_NB_BLDC_MOTORS)

// - public functions ----------------------------------------------------------
uint16_t foc_init(void) {
	uint16_t n;
	double x, w, wstep;

	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
	// fill in sine wave lookup table
	w = 0.0;
	wstep = (2*M_PI)/FOC_SIN_LUT_SIZE;
	sin_lut[0] = FOC_SIN_OFFSET; // 1st value is already given: sin(0) + offset = offset
	for(n = 1; n < FOC_SIN_LUT_SIZE; n++) {
		w += wstep;
		x = (double)FOC_SIN_AMPLITUDE * sin(w) + (double)FOC_SIN_OFFSET;
		sin_lut[n] = (uint16_t)x;
	}
	// sin lut generation tested: 2025-05-14, Martin Egli: OK
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
	
	// init motor 1
	motors[FOC_MOTOR1].TIM = FOC_BLDC_MOTOR1_TIMER;
	motors[FOC_MOTOR1].sin.u = FOC_SIN_LUT_U_START;
	motors[FOC_MOTOR1].sin.v = FOC_SIN_LUT_V_START;
	motors[FOC_MOTOR1].sin.w = FOC_SIN_LUT_W_START;
	motors[FOC_MOTOR1].speed_ctrl.tim_channel = LL_TIM_CHANNEL_CH1;
	motors[FOC_MOTOR1].x = 0;

	// init motor 2
	motors[FOC_MOTOR2].TIM = FOC_BLDC_MOTOR2_TIMER;
	motors[FOC_MOTOR2].sin.u = FOC_SIN_LUT_U_START;
	motors[FOC_MOTOR2].sin.v = FOC_SIN_LUT_V_START;
	motors[FOC_MOTOR2].sin.w = FOC_SIN_LUT_W_START;
	motors[FOC_MOTOR2].speed_ctrl.tim_channel = LL_TIM_CHANNEL_CH2;
	motors[FOC_MOTOR2].x = 0;

	return FOC_NB_BLDC_MOTORS;
}

void foc_start(void) {
	uint32_t cnt;
	LL_TIM_OC_SetCompareCH1(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.u]);
	LL_TIM_OC_SetCompareCH2(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.v]);
	LL_TIM_OC_SetCompareCH3(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.w]);
	LL_TIM_EnableAllOutputs(motors[FOC_MOTOR1].TIM);
	LL_TIM_CC_EnableChannel(motors[FOC_MOTOR1].TIM, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH3);
	LL_TIM_EnableCounter(motors[FOC_MOTOR1].TIM);

	motors[FOC_MOTOR1].speed_ctrl.tim_ccr = 0; // 10 rps

	LL_TIM_DisableIT_CC1(TIM3);
	cnt = LL_TIM_GetCounter(TIM3);
	LL_TIM_OC_SetCompareCH1(TIM3, cnt + motors[FOC_MOTOR1].speed_ctrl.tim_ccr);
	LL_TIM_ClearFlag_CC1(TIM3);
	LL_TIM_EnableIT_CC1(TIM3);
	LL_TIM_EnableCounter(TIM3);
	/*
	uint16_t tim_pwm = 0;
		HAL_TIM_Base_Start(&htim1);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, tim_pwm);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);*/

	cnt = LL_TIM_GetCounter(TIM3);
	LL_TIM_OC_SetCompareCH4(TIM3, cnt + 10000);
	LL_TIM_ClearFlag_CC4(TIM3);
	LL_TIM_EnableIT_CC4(TIM3);

	return;
}

void foc_set_speed(uint16_t mi, int16_t speed) {
	uint32_t primask;

	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);

	if(IS_MOTOR_INDEX_VALID(mi)) {
		motors[mi].speed_ctrl.speed = abs(speed);
		if(motors[mi].speed_ctrl.speed > FOC_MOTOR_MAX_SPEED) {
			motors[mi].speed_ctrl.speed = FOC_MOTOR_MAX_SPEED;
		}
		if(speed >= 0) {
			// forewards
			motors[mi].speed_ctrl.dir = FOC_MOTOR_DIR_FOREWARD;
		}
		else {
			// backwards
			motors[mi].speed_ctrl.dir = FOC_MOTOR_DIR_BACKWARD;
		}

		// CCR
		if(speed == 0) {
			primask = __get_PRIMASK();  // Save current interrupt state
			__disable_irq();            // Disable all interrupts

			motors[mi].speed_ctrl.tim_ccr = 0;
			// disable FOC_SPEED_TIM, CCRn
			if(motors[mi].speed_ctrl.tim_channel == LL_TIM_CHANNEL_CH1) {
				//LL_TIM_DisableIT_CC1(TIM3);
				LL_TIM_OC_SetMode(motors[FOC_MOTOR1].TIM, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_FORCED_INACTIVE);
				LL_TIM_OC_SetCompareCH1(motors[FOC_MOTOR1].TIM, 0);
				LL_TIM_OC_SetCompareCH2(motors[FOC_MOTOR1].TIM, 0);
				LL_TIM_OC_SetCompareCH3(motors[FOC_MOTOR1].TIM, 0);
			}
			if(motors[mi].speed_ctrl.tim_channel == LL_TIM_CHANNEL_CH2)
				LL_TIM_DisableIT_CC2(TIM3);

			__set_PRIMASK(primask);     // Restore previous interrupt state

		}
		else {
			primask = __get_PRIMASK();  // Save current interrupt state
			__disable_irq();            // Disable all interrupts

			motors[mi].speed_ctrl.tim_ccr = ((FOC_SPEED_TIM_FREQ/FOC_NB_STEPS_PER_FULL_ROTATION)*100)/motors[mi].speed_ctrl.speed;
			if(motors[mi].speed_ctrl.tim_channel == LL_TIM_CHANNEL_CH1)
				LL_TIM_OC_SetMode(motors[FOC_MOTOR1].TIM, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_PWM1);
				LL_TIM_EnableIT_CC1(TIM3);
//			if(motors[mi].speed_ctrl.tim_channel == LL_TIM_CHANNEL_CH2)
//				LL_TIM_EnableIT_CC2(TIM3);

			__set_PRIMASK(primask);     // Restore previous interrupt state
		}
	}
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
}

static volatile uint32_t m1_cnt = 0, m1_state = 0;
static volatile int16_t m1_speed = 1;
void foc_ctrl_process(void) {
	uint32_t cnt;

	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);

	if (LL_TIM_IsActiveFlag_CC1(TIM3)) {
		//LL_TIM_ClearFlag_CC1(TIM3);

		LL_TIM_DisableIT_CC1(TIM3);

		motors[FOC_MOTOR1].x++;
		//global_event |= 1;

		/*
		if(m1_cnt == 0) {
			m1_cnt = 1000;

			if(m1_state == 3) {
				m1_state = 0;
			}
			else {
				m1_state++;
			}
		}
		else {
			m1_cnt--;
		}
		if(m1_state == 0) {
			// foreward
			motors[FOC_MOTOR1].sin.u = NEXT_INDEX_FOREWARD(motors[FOC_MOTOR1].sin.u);
			motors[FOC_MOTOR1].sin.v = NEXT_INDEX_FOREWARD(motors[FOC_MOTOR1].sin.v);
			motors[FOC_MOTOR1].sin.w = NEXT_INDEX_FOREWARD(motors[FOC_MOTOR1].sin.w);
			LL_TIM_OC_SetMode(motors[FOC_MOTOR1].TIM, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_PWM1);
			LL_TIM_OC_SetCompareCH1(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.u]);
			LL_TIM_OC_SetCompareCH2(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.v]);
			LL_TIM_OC_SetCompareCH3(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.w]);
		}
		else if(m1_state == 1) {
			// stop
			LL_TIM_OC_SetMode(motors[FOC_MOTOR1].TIM, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_FORCED_INACTIVE);
			LL_TIM_OC_SetCompareCH1(motors[FOC_MOTOR1].TIM, 0);
			LL_TIM_OC_SetCompareCH2(motors[FOC_MOTOR1].TIM, 0);
			LL_TIM_OC_SetCompareCH3(motors[FOC_MOTOR1].TIM, 0);
		}
		else if(m1_state == 2) {
			// backwards
			motors[FOC_MOTOR1].sin.u = NEXT_INDEX_BACKWARD(motors[FOC_MOTOR1].sin.u);
			motors[FOC_MOTOR1].sin.v = NEXT_INDEX_BACKWARD(motors[FOC_MOTOR1].sin.v);
			motors[FOC_MOTOR1].sin.w = NEXT_INDEX_BACKWARD(motors[FOC_MOTOR1].sin.w);
			LL_TIM_OC_SetMode(motors[FOC_MOTOR1].TIM, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_PWM1);
			LL_TIM_OC_SetCompareCH1(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.u]);
			LL_TIM_OC_SetCompareCH2(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.v]);
			LL_TIM_OC_SetCompareCH3(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.w]);
		}
		else { //if(m1_state == 3) {
			// stop
			LL_TIM_OC_SetMode(motors[FOC_MOTOR1].TIM, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_FORCED_INACTIVE);
			LL_TIM_OC_SetCompareCH1(motors[FOC_MOTOR1].TIM, 0);
			LL_TIM_OC_SetCompareCH2(motors[FOC_MOTOR1].TIM, 0);
			LL_TIM_OC_SetCompareCH3(motors[FOC_MOTOR1].TIM, 0);
		}*/
/*
		if(m1_cnt == 0) {
			m1_cnt = 10;
			if(m1_state == 0) {
				if(m1_speed < 1000) {
					m1_speed++;
				}
				else {
					m1_state = 1;
				}
			}
			else {
				if(m1_speed > -1000) {
					m1_speed--;
				}
				else {
					m1_state = 0;
				}
			}
			foc_set_speed(FOC_MOTOR1, m1_speed);
		}
		else {
			m1_cnt--;
		}
*/
		if(motors[FOC_MOTOR1].speed_ctrl.tim_ccr == 0) {
			LL_TIM_OC_SetMode(motors[FOC_MOTOR1].TIM, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_FORCED_INACTIVE);
			LL_TIM_OC_SetCompareCH1(motors[FOC_MOTOR1].TIM, 0);
			LL_TIM_OC_SetCompareCH2(motors[FOC_MOTOR1].TIM, 0);
			LL_TIM_OC_SetCompareCH3(motors[FOC_MOTOR1].TIM, 0);
		}
		else {
			if(motors[FOC_MOTOR1].speed_ctrl.dir == FOC_MOTOR_DIR_FOREWARD) {
				motors[FOC_MOTOR1].sin.u = NEXT_INDEX_FOREWARD(motors[FOC_MOTOR1].sin.u);
				motors[FOC_MOTOR1].sin.v = NEXT_INDEX_FOREWARD(motors[FOC_MOTOR1].sin.v);
				motors[FOC_MOTOR1].sin.w = NEXT_INDEX_FOREWARD(motors[FOC_MOTOR1].sin.w);
			}
			else {
				motors[FOC_MOTOR1].sin.u = NEXT_INDEX_BACKWARD(motors[FOC_MOTOR1].sin.u);
				motors[FOC_MOTOR1].sin.v = NEXT_INDEX_BACKWARD(motors[FOC_MOTOR1].sin.v);
				motors[FOC_MOTOR1].sin.w = NEXT_INDEX_BACKWARD(motors[FOC_MOTOR1].sin.w);
			}
			LL_TIM_OC_SetMode(motors[FOC_MOTOR1].TIM, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_PWM1);
			LL_TIM_OC_SetCompareCH1(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.u]);
			LL_TIM_OC_SetCompareCH2(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.v]);
			LL_TIM_OC_SetCompareCH3(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.w]);
		}

		cnt = LL_TIM_GetCounter(TIM3);
		LL_TIM_OC_SetCompareCH1(TIM3, cnt + motors[FOC_MOTOR1].speed_ctrl.tim_ccr);
		LL_TIM_ClearFlag_CC1(TIM3);
		LL_TIM_EnableIT_CC1(TIM3);
	}

	if (LL_TIM_IsActiveFlag_CC4(TIM3)) {
		//LL_TIM_ClearFlag_CC4(TIM3);

		global_event |= 2;
		cnt = LL_TIM_GetCounter(TIM3);
		LL_TIM_OC_SetCompareCH4(TIM3, cnt + 10000);
		LL_TIM_ClearFlag_CC4(TIM3);
		LL_TIM_EnableIT_CC4(TIM3);
	}

	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
}

/*
uint32_t primask;

primask = __get_PRIMASK();  // Save current interrupt state
__disable_irq();            // Disable all interrupts

// --- Critical Section Start ---
/* Your critical code here * /
// --- Critical Section End ---

__set_PRIMASK(primask);     // Restore previous interrupt state
*/
