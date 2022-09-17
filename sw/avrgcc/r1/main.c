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

/* - public variables ------------------------------------------------------- */
static uint8_t sine_lookup[] = {127, 143, 159, 175, 190, 204, 216, 227, 236, 244, 249, 252, 254, 252, 249, 244, 236, 227, 216, 204, 190, 175, 159, 143, 127, 110, 94, 78, 63, 49, 37, 26, 17, 9, 4, 1, 0, 1, 4, 9, 17, 26, 37, 49, 63, 78, 94, 110};
static const uint8_t nb_sine_steps = NB_ELEMENTS(sine_lookup);

typedef struct
{
    uint8_t u, v, w;
} motor_index_t;
static motor_index_t pwm_index_a = {.u = 0, .v = 16, .w = 32};

volatile uint8_t global_events;
volatile uint8_t global_display_events;
volatile uint8_t global_button_events;

static uint8_t inc_uint8_max(uint8_t value, uint8_t max)
{
    if (value < (max - 1))
    {
        return value + 1;
    }
    return 0;
}

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

    motor_start();
    motorA_update_pwm(sine_lookup[pwm_index_a.u], sine_lookup[pwm_index_a.v], sine_lookup[pwm_index_a.w]);
    wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_16MS, EV_UPDATE_PWM);

    sei();

    // event loop
    while (1)
    {
        if (local_events & EV_UPDATE_PWM)
        {
            pwm_index_a.u = inc_uint8_max(pwm_index_a.u, nb_sine_steps);
            pwm_index_a.v = inc_uint8_max(pwm_index_a.v, nb_sine_steps);
            pwm_index_a.w = inc_uint8_max(pwm_index_a.w, nb_sine_steps);
            motorA_update_pwm(sine_lookup[pwm_index_a.u], sine_lookup[pwm_index_a.v], sine_lookup[pwm_index_a.w]);
            wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_16MS, EV_UPDATE_PWM);
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
