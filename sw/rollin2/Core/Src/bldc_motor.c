/**
 * Martin Egli
 * 2025-06-28
 * bldc motor module
 */

#include "bldc_motor.h"

uint16_t bldc_motor_init(bldc_motor_t *m, bldc_driver_t *d, angle_sens_t *as) {
    if(m == NULL) {
        // error, invalid motor
        return false;
    }
    if(d == NULL) {
        // error, invalid bldc driver, this is mandatory
        return false;
    }
    m->d = d;
    if(as == NULL) {
        // error, invalid angle sensor, this is mandatory
        return false;
    }
	m->as = as;

    m->status &= ~BLDC_MOTOR_STATUS_ENABLED;
    m->limit.voltage = 0.0f;
    m->pid.kp = 0.0f;
    m->pid.ki = 0.0f;
    m->pid.kd = 0.0f;
    return true;
}

uint16_t bldc_motor_set_pid(bldc_motor_t *m, float kp, float ki, float kd) {
	if(m == NULL) {
        // error, invalid motor
        return false;
    }
    m->pid.kp = kp;
    m->pid.ki = ki;
    m->pid.kd = kd;
    return true;
}

uint16_t bldc_motor_set_voltage_limit(bldc_motor_t *m, float voltage_limit) {
	if(m == NULL) {
        // error, invalid motor
        return false;
    }
    m->limit.voltage = voltage_limit;
    return true;
}

uint16_t bldc_motor_set_target_speed(bldc_motor_t *m, float speed) {
	if(m == NULL) {
        // error, invalid motor
        return false;
    }
    m->target.speed = speed;
    return true;
}

/* set template
uint16_t bldc_motor_set_target_x(bldc_motor_t *m, float x) {
    if(m == NULL) {
        // error, invalid motor
        return false;
    }
    m-> = x;
    return true;
}
*/

uint16_t bldc_motor_enable(bldc_motor_t *m) {
    if(m == NULL) {
        // error, invalid motor
        return false;
    }
    if(bldc_driver_enable(m->d) == false) {
        // error, could not enable bldc driver
        return false;
    }
    // OK, motor + driver enabled
    m->status |= BLDC_MOTOR_STATUS_ENABLED;
    return true;
    
}
uint16_t bldc_motor_disable(bldc_motor_t *m) {
    if(m == NULL) {
        // error, invalid motor
        return false;
    }
    m->status &= ~BLDC_MOTOR_STATUS_ENABLED;
    return bldc_driver_disable(m->d);
}

uint16_t bldc_motor_process_foc(bldc_motor_t *m) {
    if(m == NULL) {
        // error, invalid motor
        return false;
    }
    if((m->status & BLDC_MOTOR_STATUS_ENABLED) == 0) {
        // is disabled, stop here
        return false;
    }

    // get angle
    if(angle_sensor_get(m->as) == false) {
        // error, no valid angle value
    	return false;
    }

    // OK
    return true;
}

uint16_t bldc_motor_log(char *str);

