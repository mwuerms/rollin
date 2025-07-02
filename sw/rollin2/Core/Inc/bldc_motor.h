/**
 * Martin Egli
 * 2025-06-29
 * bldc motor module
 */
#ifndef _BLDC_MOTOR_H_ 
#define _BLDC_MOTOR_H_ 

#include <stdint.h>
#include <stdbool.h>
#include "project.h"
#include "bldc_driver.h"
#include "angle_sensor.h"

#ifndef STM32F103xB
#error wrong mcu
#else
#warning info mcu defined STM32F103xB
#endif

typedef struct {
    bldc_driver_t *d;
    angle_sens_t *as;
    struct {
        float speed;
        float angle;
    } target;
    struct {
        float sens_angle;
        float el_angle;
    } get;
    struct {
        float voltage;
    } limit;
    struct {
        float kp;
        float ki;
        float integrator;
        float kd;
        float differentiator;
    } pid;
    uint16_t status;
    uint16_t ctrl_type;
} bldc_motor_t;

#define BLDC_MOTOR_STATUS_ENABLED (0x0001)

#define BLDC_MOTOR_CTRL_TYPE_NONE (0)
#define BLDC_MOTOR_CTRL_TYPE_VELOCITY_OPENLOOP (1)
#define BLDC_MOTOR_CTRL_TYPE_ANGLE_OPENLOOP (2)
#define BLDC_MOTOR_CTRL_TYPE_VELOCITY (3)
#define BLDC_MOTOR_CTRL_TYPE_ANGLE (4)
/*#define BLDC_MOTOR_CTRL_TYPE_VOLTAGE (0)
#define BLDC_MOTOR_CTRL_TYPE_DC_CURRENT (0)
#define BLDC_MOTOR_CTRL_TYPE_FOC_CURRENT (0)
#define BLDC_MOTOR_CTRL_TYPE_TORQUE (0)
#define BLDC_MOTOR_CTRL_TYPE_ANGLE (0)*/


uint16_t bldc_motor_init(bldc_motor_t *m, bldc_driver_t *d, angle_sens_t *as);

uint16_t bldc_motor_set_pid(bldc_motor_t *m, float kp, float ki, float kd);
uint16_t bldc_motor_set_voltage_limit(bldc_motor_t *m, float voltage_limit);
uint16_t bldc_motor_set_target_speed(bldc_motor_t *m, float speed);
//uint16_t bldc_motor_set_pid(bldc_motor_t *m, 

uint16_t bldc_motor_enable(bldc_motor_t *m);
uint16_t bldc_motor_disable(bldc_motor_t *m);
uint16_t bldc_motor_process_foc(bldc_motor_t *m);
uint16_t bldc_motor_log(char *str);

#endif /* _BLDC_MOTOR_H_ */
