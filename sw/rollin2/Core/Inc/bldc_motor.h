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
        float speed_rot_s; /// target speed in rotation / s
        float angle_deg; /// target angle in deg
        float angle_deg_s; /// target angle velocity in deg / s
    } target;
    struct {
        float angle_deg; /// current angle in deg
        float angle_deg_old;
        float delta_angle_deg;
		float angle_deg_s; /// current angle velocity in deg / s
        float el_angle;
    } current;
    struct {
        float shaft_angle_deg; /// 
    } calc; /// for calculating purposes
    struct {
        float voltage;
    } limit;
    struct {
        uint16_t pole_pairs;
        float kv;
        float coil_resistance;
    } motor;
    struct {
        float kp;
        float ki;
        float integrator;
        float kd;
        float differentiator;
    } pid;
    struct {
    	float voltage_q;
    } set;
    struct {
        uint16_t status;
        uint16_t type;
    } ctrl;
} bldc_motor_t;

#define BLDC_MOTOR_STATUS_ENABLED (0x0001)

#define BLDC_MOTOR_CTRL_TYPE_VELOCITY_OPENLOOP (0)
#define BLDC_MOTOR_CTRL_TYPE_ANGLE_OPENLOOP (1)
#define BLDC_MOTOR_CTRL_TYPE_VELOCITY (2)
#define BLDC_MOTOR_CTRL_TYPE_ANGLE (3)
/*#define BLDC_MOTOR_CTRL_TYPE_VOLTAGE (4)
#define BLDC_MOTOR_CTRL_TYPE_FOC_CURRENT (5)
#define BLDC_MOTOR_CTRL_TYPE_TORQUE (6)
#define BLDC_MOTOR_CTRL_TYPE_ANGLE (7)*/

extern float dbg_uu;
extern float dbg_uv;
extern float dbg_uw;

extern uint16_t statangle_u;
extern uint16_t statangle_v;
extern uint16_t statangle_w;
uint16_t bldc_motor_init(bldc_motor_t *m, bldc_driver_t *d, angle_sens_t *as);
uint16_t bldc_motor_set_motor_parameters(bldc_motor_t *m, uint16_t pp, float kv, float coil_res);
uint16_t bldc_motor_set_ctrl_type(bldc_motor_t *m, uint16_t ctype);
uint16_t bldc_motor_set_pid(bldc_motor_t *m, float kp, float ki, float kd);
uint16_t bldc_motor_set_voltage_limit(bldc_motor_t *m, float voltage_limit);
uint16_t bldc_motor_set_target_speed(bldc_motor_t *m, float speed);
uint16_t bldc_motor_set_target_angle_deg(bldc_motor_t *m, float angle_deg);
//uint16_t bldc_motor_set_pid(bldc_motor_t *m, 

uint16_t bldc_motor_enable(bldc_motor_t *m);
uint16_t bldc_motor_disable(bldc_motor_t *m);
uint16_t bldc_motor_move(bldc_motor_t *m, float dt);

#endif // _BLDC_MOTOR_H_
