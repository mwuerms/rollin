/**
 * Martin Egli
 * 2022-06-06
 *
 * Timer1 as variable PWM output
 */

#ifndef _PWM_H_
#define _PWM_H_
/* - includes --------------------------------------------------------------- */

#include <stdint.h>

// - defines -------------------------------------------------------------------
#define TIMER1_PRESCALER (1)
#define TIMER1_TOP (255)
#define TIMER1_TICK_RATE (F_CPU / (TIMER1_PRESCALER * (TIMER1_TOP + 1)))
// currently: 16 MHz /(1 * (255 + 1)) = 62.5 kHz = 16 us

// - public MACROS -------------------------------------------------------------
void motor_start(void);
// idea: set torque in 0 (min, 0) ... 1 (max, 256) in uint8_t, use fixed point to get result in uint16_t, use high byte
void motorA_update_pwm(uint8_t pwm_u, uint8_t pwm_v, uint8_t pwm_w);
void motorB_update_pwm(uint8_t pwm_u, uint8_t pwm_v, uint8_t pwm_w);
void motor_send_event_after_nb_ticks(uint16_t nb_ticks, uint8_t event);

void motorA_speed(int16_t spd_a);
void motor_stop(void);

#endif /* _MACROS_H_ */
