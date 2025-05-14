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
#define FOC_SIN_LUT_NB_VALUES (12UL)
#define FOC_SIN_LUT_SIZE (FOC_BLDC_MOTOR_NB_COILS*FOC_SIN_LUT_NB_VALUES)
#define FOC_SIN_LUT_MAX_INDEX (FOC_SIN_LUT_SIZE - 1)
#define FOC_SIN_LUT_U_START (0 * FOC_SIN_LUT_NB_VALUES)
#define FOC_SIN_LUT_V_START (1 * FOC_SIN_LUT_NB_VALUES)
#define FOC_SIN_LUT_W_START (2 * FOC_SIN_LUT_NB_VALUES)
#define FOC_SIN_OFFSET ((FOC_PWM_MAX+1)/2) // 1/2 of max
#define FOC_SIN_AMPLITUDE (FOC_PWM_MAX/2) // this determines the current through the coils

#define FOC_NB_STEPS_PER_FULL_ROTATION (FOC_SIN_LUT_SIZE * FOC_BLDC_MOTOR_NB_POLES / FOC_BLDC_MOTOR_NB_COILS)

// - private variables ---------------------------------------------------------
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

	// init motor 2
	motors[FOC_MOTOR2].TIM = FOC_BLDC_MOTOR2_TIMER;
	motors[FOC_MOTOR2].sin.u = FOC_SIN_LUT_U_START;
	motors[FOC_MOTOR2].sin.v = FOC_SIN_LUT_V_START;
	motors[FOC_MOTOR2].sin.w = FOC_SIN_LUT_W_START;

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

	LL_TIM_DisableIT_CC1(TIM3);
	cnt = LL_TIM_GetCounter(TIM3);
	LL_TIM_OC_SetCompareCH1(TIM3, cnt + 1000);
	LL_TIM_ClearFlag_CC1(TIM3);
	LL_TIM_EnableIT_CC1(TIM3);
	LL_TIM_EnableCounter(TIM3);
	/*
	uint16_t tim_pwm = 0;
		HAL_TIM_Base_Start(&htim1);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, tim_pwm);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);*/
	return;
}


void foc_ctrl_process(void) {
	uint32_t cnt;

	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);

	motors[FOC_MOTOR1].sin.u = NEXT_INDEX_FOREWARD(motors[FOC_MOTOR1].sin.u);
	motors[FOC_MOTOR1].sin.v = NEXT_INDEX_FOREWARD(motors[FOC_MOTOR1].sin.v);
	motors[FOC_MOTOR1].sin.w = NEXT_INDEX_FOREWARD(motors[FOC_MOTOR1].sin.w);
	LL_TIM_OC_SetCompareCH1(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.u]);
	LL_TIM_OC_SetCompareCH2(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.v]);
	LL_TIM_OC_SetCompareCH3(motors[FOC_MOTOR1].TIM, (uint32_t)sin_lut[motors[FOC_MOTOR1].sin.w]);

	LL_TIM_DisableIT_CC1(TIM3);
	cnt = LL_TIM_GetCounter(TIM3);
	LL_TIM_OC_SetCompareCH1(TIM3, cnt + 1000);
	LL_TIM_ClearFlag_CC1(TIM3);
	LL_TIM_EnableIT_CC1(TIM3);

	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
}
