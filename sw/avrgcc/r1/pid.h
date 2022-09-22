/**
 * Martin Egli
 * 2022-09-21
 *
 * pid controller module
 */

#ifndef _PID_H_
#define _PID_H_

// - include -------------------------------------------------------------------
#include <stdint.h>

// - public functions ----------------------------------------------------------

void pid_init(float kp, float ki, float kd);

#endif // _PID_H_
