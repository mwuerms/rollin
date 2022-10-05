/**
 * Martin Egli
 * 2022-09-21
 *
 * pid controller module
 */

// - include -------------------------------------------------------------------
#include "pid.h"

// - private variables ---------------------------------------------------------
static float error_last, out_value;
static float k_p, k_i, k_d;
static float i_term, d_term;

// - public functions ----------------------------------------------------------

void pid_init(float kp, float ki, float kd)
{
    error_last = 0.0f;
    out_value = 0.0f;
    k_p = kp;
    k_i = ki;
    k_d = kd;
}

float pid_exec(float error, float delta_t)
{
    out_value = 0.0f;
    // proportional
    if (k_p != 0.0f)
    {
        out_value += k_p * error;
    }

    // integral
    if (k_i != 0.0f)
    {
        i_term = k_i * (error * delta_t) + i_term;
        out_value += i_term;
    }

    // differential
    if (k_p != 0.0f)
    {
        d_term = k_d * (error / delta_t) - error_last;
        out_value += d_term;
    }

    error_last = error;
    return out_value;
}
