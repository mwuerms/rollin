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

// - public MACROS -------------------------------------------------------------
/*void pwm_init(void);

#define PWM_MAX_FREQUENCY 10000 // max. 10 kHz
#define PWM_MAX_DUTY_CYCLE 1000 // in 0.1 % steps

void pwm_start(uint16_t set_freq, uint16_t set_pulse_witdh); // pwm_on()
void pwm_start_sweep_duty_cycle(uint16_t set_freq, uint16_t duty_cycle_start, uint16_t duty_cycle_stop);
void pwm_stop(void); // pwm_off()
*/

void motor_start(void);
void motorA_update_pwm(uint8_t pwm_u, uint8_t pwm_v, uint8_t pwm_w);
void motorB_update_pwm(uint8_t pwm_u, uint8_t pwm_v, uint8_t pwm_w);
void motor_stop(void);

#endif /* _MACROS_H_ */
