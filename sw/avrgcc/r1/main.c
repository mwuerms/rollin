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

#include "project.h"
#include "macrolib.h"
#include "mcusleep.h"
#include "motor.h"
#include "wdt.h"
#include "mpu6500.h"
#include "uart.h"
#include "string_buffer.h"

/* - defines ---------------------------------------------------------------- */
typedef struct
{
    uint8_t u, v, w;
} motor_index_t;

/* - public variables ------------------------------------------------------- */
static uint8_t sine_lookup[] = {127, 135, 143, 151, 159, 167, 175, 183, 190, 197, 204, 210, 216, 222, 227, 232, 236, 240, 244, 247, 249, 251, 252, 253, 254, 253, 252, 251, 249, 247, 244, 240, 236, 232, 227, 222, 216, 210, 204, 197, 190, 183, 175, 167, 159, 151, 143, 135, 127, 118, 110, 102, 94, 86, 78, 70, 63, 56, 49, 43, 37, 31, 26, 21, 17, 13, 9, 6, 4, 2, 1, 0, 0, 0, 1, 2, 4, 6, 9, 13, 17, 21, 26, 31, 37, 43, 49, 56, 63, 70, 78, 86, 94, 102, 110, 118};
static motor_index_t pwm_index_a = {.u = 0, .v = 32, .w = 64};
// static uint8_t sine_lookup[] = {127, 143, 159, 175, 190, 204, 216, 227, 236, 244, 249, 252, 254, 252, 249, 244, 236, 227, 216, 204, 190, 175, 159, 143, 127, 110, 94, 78, 63, 49, 37, 26, 17, 9, 4, 1, 0, 1, 4, 9, 17, 26, 37, 49, 63, 78, 94, 110};
// static motor_index_t pwm_index_a = {.u = 0, .v = 16, .w = 32};
// static uint8_t sine_lookup[] = {127, 159, 190, 216, 236, 249, 254, 249, 236, 216, 190, 159, 127, 94, 63, 37, 17, 4, 0, 4, 17, 37, 63, 94};
// static motor_index_t pwm_index_a = {.u = 0, .v = 8, .w = 16};
// static uint8_t sine_lookup[] = {127, 190, 236, 254, 236, 190, 127, 63, 17, 0, 17, 63};
// static motor_index_t pwm_index_a = {.u = 0, .v = 4, .w = 8};

static const uint8_t nb_sine_steps = NB_ELEMENTS(sine_lookup);

volatile uint8_t global_events;
volatile uint8_t global_display_events;
volatile uint8_t global_button_events;

static uint8_t inc_uint8_max(uint8_t value, uint8_t max)
{
    value++;
    if (value >= max)
        return 0;
    return value; /*
     if (value < (max - 1))
     {
         return value + 1;
     }
     return 0;*/
}

#define DBG_PIN0_OUT() (DDRB |= _BV(4))
#define DBG_PIN0_SET() (PORTB |= _BV(4))
#define DBG_PIN0_CLR() (PORTB &= ~_BV(4))

uint8_t sens_buffer[16];
int16_t acc_x, acc_y, gyr_z;
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
    mpu6500_init();
    mpu6500_set_config();

    motor_start();
    motorA_update_pwm(sine_lookup[pwm_index_a.u], sine_lookup[pwm_index_a.v], sine_lookup[pwm_index_a.w]);
    wdtTimer_StartTimeout(15, WDT_TIMER_INTERVAL_500MS, EV_READ_SENSOR);

    DBG_PIN0_OUT();
    DBG_PIN0_CLR();

    sei();

    // event loop
    while (1)
    {
        if (local_events & EV_UPDATE_PWM)
        {
            // found 7 pole pairs
            DBG_PIN0_SET();
            // wdtTimer_StartTimeout(1, WDT_TIMER_INTERVAL_16MS, EV_UPDATE_PWM);
            pwm_index_a.u = inc_uint8_max(pwm_index_a.u, nb_sine_steps);
            pwm_index_a.v = inc_uint8_max(pwm_index_a.v, nb_sine_steps);
            pwm_index_a.w = inc_uint8_max(pwm_index_a.w, nb_sine_steps);
            // motorA_update_pwm(sine_lookup[pwm_index_a.u] * 0.25, sine_lookup[pwm_index_a.v] * 0.25, sine_lookup[pwm_index_a.w] * 0.25);
            // motorB_update_pwm(sine_lookup[pwm_index_a.u] * 0.25, sine_lookup[pwm_index_a.v] * 0.25, sine_lookup[pwm_index_a.w] * 0.25);
            //  motorA_update_pwm(sine_lookup[pwm_index_a.u] * 0.5, sine_lookup[pwm_index_a.v] * 0.5, sine_lookup[pwm_index_a.w] * 0.5);
            //  motorB_update_pwm(sine_lookup[pwm_index_a.u] * 0.5, sine_lookup[pwm_index_a.v] * 0.5, sine_lookup[pwm_index_a.w] * 0.5);
            motorA_update_pwm(sine_lookup[pwm_index_a.u] * 1, sine_lookup[pwm_index_a.v] * 1, sine_lookup[pwm_index_a.w] * 1);
            motorB_update_pwm(sine_lookup[pwm_index_a.u] * 1, sine_lookup[pwm_index_a.v] * 1, sine_lookup[pwm_index_a.w] * 1);
            DBG_PIN0_CLR();
        }

        if (local_events & EV_READ_SENSOR)
        {
            wdtTimer_StartTimeout(15, WDT_TIMER_INTERVAL_500MS, EV_READ_SENSOR);
            mpu6500_read_sensor_data(sens_buffer, 14);
            acc_x = (int16_t)(sens_buffer[0] << 8) + sens_buffer[1];
            acc_y = (int16_t)(sens_buffer[2] << 8) + sens_buffer[3];
            gyr_z = (int16_t)(sens_buffer[12] << 8) + sens_buffer[13];
            string_buffer_new();
            string_buffer_append_int16(acc_x);
            string_buffer_append_separator();
            string_buffer_append_int16(acc_y);
            string_buffer_send_uart();
            string_buffer_new();
            string_buffer_append_separator();
            string_buffer_append_int16(gyr_z);
            string_buffer_append_new_line();
            string_buffer_send_uart();
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
