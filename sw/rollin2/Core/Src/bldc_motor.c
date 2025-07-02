/**
 * Martin Egli
 * 2025-06-28
 * bldc motor module
 */

#include "bldc_motor.h"
#include <math.h>

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
    m->target.speed_rot_s = speed;
    m->target.angle_deg_s = 360.0f * speed;
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

uint16_t bldc_motor_angle_velocity_pid(bldc_motor_t *m, float dt) {
	float error = m->target.angle_deg_s - m->current.angle_deg_s;

	// out = kp * err + ki * err * dt + old_i + kd * err / dt;
	m->pid.integrator += m->pid.ki * error * dt;
	// limit
	if(m->pid.integrator > +m->limit.voltage) m->pid.integrator = +m->limit.voltage;
	if(m->pid.integrator < -m->limit.voltage) m->pid.integrator = -m->limit.voltage;

	m->set.voltage_q = m->pid.kp * error + m->pid.integrator;
	if(m->set.voltage_q > +m->limit.voltage) m->set.voltage_q = +m->limit.voltage;
	if(m->set.voltage_q < -m->limit.voltage) m->set.voltage_q = -m->limit.voltage;
}

uint16_t bldc_motor_update(bldc_motor_t *m, float dt) {
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
    m->current.angle_deg = m->as->angle_deg;

    // calc angle velocity
    m->current.delta_angle_deg = (m->current.angle_deg - m->current.angle_deg_old);
    // limit
    if(m->current.delta_angle_deg >  180.0f) m->current.delta_angle_deg -= 360.0f;
	if(m->current.delta_angle_deg < -180.0f) m->current.delta_angle_deg += 360.0f;
	m->current.angle_deg_s = m->current.delta_angle_deg / dt;
    m->current.angle_deg_old = m->current.angle_deg;

    // process PID
    bldc_motor_angle_velocity_pid(m, dt);

    // calculate phase voltages u, v, w, each 120° apart
    float angle_rad = (m->current.angle_deg * 2*M_PI) / 360.0f;
    float uu = 0.5f + 0.5f * m->set.voltage_q * sinf(angle_rad);
    float uv = 0.5f + 0.5f * m->set.voltage_q * sinf(angle_rad - ((120.0f * 2*M_PI) / 360.0f));
    float uw = 0.5f + 0.5f * m->set.voltage_q * sinf(angle_rad - ((240.0f * 2*M_PI) / 360.0f));

    bldc_driver_set_phase_voltages(m->d, uu, uv, uw);
    // OK
    return true;
}

uint16_t bldc_motor_log(char *str);

