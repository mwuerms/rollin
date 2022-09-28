/**
 * Martin Egli
 * 2022-06-06
 *
 * project on github: https://github.com/mwuerms/pwmgen
 * avr-gcc see: /usr/lib/avr
 * http://www.nongnu.org/avr-libc/user-manual/using_tools.html
 */

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include <avr/interrupt.h>
#include <string.h>
#include <math.h>

#include "project.h"
#include "macrolib.h"
#include "mcusleep.h"
#include "motor.h"
#include "wdt.h"
#include "mpu6500.h"
#include "uart.h"
#include "string_buffer.h"
#include "pid.h"
#include "nunchuck.h"

/* - defines ---------------------------------------------------------------- */

/* - public variables ------------------------------------------------------- */
volatile uint8_t global_events;
volatile uint8_t global_display_events;
volatile uint8_t global_button_events;

float pid_return_value;

#define DBG_PIN0_OUT() (DDRB |= _BV(4))
#define DBG_PIN0_SET() (PORTB |= _BV(4))
#define DBG_PIN0_CLR() (PORTB &= ~_BV(4))

uint8_t sens_buffer[16];
int16_t acc_x, acc_y, gyr_z;

float angle_acc, angle_gyr, angle_deg, angle_deg_prev, angle_error_deg;

int16_t angle_index, motor_index_error, motor_index = 0;
#define MOTOR_INDEX_MAX ((7 * 48) - 1)
#define MOTOR_INDEX_MIN -(7 * 48)

#define RAD_TO_DEG (180.0f / M_PI)

int8_t speed;

/**
 * main loop
 */
int main(void)
{
    uint8_t local_events = 0;
    // init
    sleep_mode_init();
    use_sleep_mode(ACTIVE);

    uart_Init(UBRR_VAL_57600);
    uart_puts("rollin am start\n");

    // pid_init(2.5f, 0.0f, 0.0f); // not bad
    pid_init(1.5f, 0.0f, 0.003f);

    mpu6500_init();
    mpu6500_set_config();

    nunchuck_start_unencrypred();
    motor_start();
    // motor_send_event_after_nb_ticks(625, EV_READ_SENSOR); // 10 ms
    // motor_send_event_after_nb_ticks(468, EV_READ_SENSOR); // 7.5 ms
    motor_send_event_after_nb_ticks(62500 / 2, EV_UPDATE_PWM); // 500 ms

    DBG_PIN0_OUT();
    DBG_PIN0_CLR();

    angle_deg_prev = 0.0f;
    motor_index = 0;
    speed = 0;

    sei();

    // event loop
    while (1)
    {
        if (local_events & EV_UPDATE_PWM)
        {
            motor_send_event_after_nb_ticks(62500 / 2, EV_UPDATE_PWM); // 500 ms
            // found 7 pole pairs
            DBG_PIN0_SET();
            wdtTimer_StartTimeout(60, WDT_TIMER_INTERVAL_1S, EV_UPDATE_PWM);

            motor_set_speed(speed);
            string_buffer_new();
            string_buffer_append_int8(speed);
            string_buffer_append_new_line();
            string_buffer_send_uart();

            speed++;
            if (speed > 20)
                speed = -20;

            DBG_PIN0_CLR();
        }

        if (local_events & EV_READ_SENSOR)
        {
            // wdtTimer_StartTimeout(2, WDT_TIMER_INTERVAL_16MS, EV_READ_SENSOR);
            // motor_send_event_after_nb_ticks(625, EV_READ_SENSOR); // 10 ms
            motor_send_event_after_nb_ticks(468, EV_READ_SENSOR); // 7.5 ms

            // get sensor values
            mpu6500_read_sensor_data(sens_buffer, 14);
            acc_x = (int16_t)(sens_buffer[0] << 8) + sens_buffer[1];
            acc_y = (int16_t)(sens_buffer[2] << 8) + sens_buffer[3];
            gyr_z = (int16_t)(sens_buffer[12] << 8) + sens_buffer[13];

            // calc angles
            angle_acc = (float)atan2((double)acc_x, (double)acc_y);
            angle_acc *= RAD_TO_DEG;
            // angle_gyr = (((float)gyr_z) / 250.0f) * 0.010f; // 0.0477455f; // gyr_z_rate_dps * sample_rate_ps
            angle_gyr = (((float)gyr_z) / 250.0f) * 0.0075f; // gyr_z_rate_dps * sample_rate_ps
            angle_deg = 0.90f * (angle_deg_prev + angle_gyr) + 0.10f * angle_acc;
            angle_deg_prev = angle_deg;

            angle_error_deg = 0.0f - angle_deg;
            // pid_return_value = pid_exec(angle_error_deg, 0.01f);
            pid_return_value = pid_exec(angle_error_deg, 1); // 0.0075f);

            speed = (int8_t)pid_return_value;

            string_buffer_new();
            string_buffer_append_float(angle_deg, 3);
            /*string_buffer_append_separator();
            string_buffer_append_float(angle_error_deg, 3);
            string_buffer_append_separator();
            string_buffer_append_float(pid_return_value, 3);*/
            string_buffer_append_separator();
            string_buffer_append_int8(speed);
            string_buffer_append_new_line();
            string_buffer_send_uart();

            // angle_index = ((int16_t)angle_deg) * 8;
            /*if (speed > 5)
                speed = 5;
            if (speed < -5)
                speed = -5;*/
            motor_set_speed(speed);

            nunchuck_read_raw(sens_buffer, 6);
        }

        while (1)
        {
            cli();
            // check if there is a new event
            local_events = global_events;

            global_events = 0;
            if (local_events)
            {
                sei();
                break;
            }
            // wait here for new event
            enter_sleep_mode();
        }
    }
    return (0);
}
