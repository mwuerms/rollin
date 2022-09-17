/**
 * Martin Egli
 * 2022-06-06
 *
 * generate variable pwm signal:
 * 1 ... 10 kHz, 0.1 ... 99.9 %
 *
 * Timer1 as variable PWM output
 * periode (TOP, frequency):
 *  ICR1
 * outputs(same frequency, different duty cycle):
 *  OC1A
 *  OC1B
 *
 * f_{pwm} = f_{clkI/O}/(N*(1+TOP))
 * f_{clkI/O} = F_CPU = 8000000
 * N: prescalser = 1 (1, 8, 64, 256, or 1024)
 *
 */
#warning _TODO frequency sweep
#warning _TODO duty cycle sweep

// - includes ------------------------------------------------------------------
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "project.h"
#include "io.h"
#include "motor.h"
#include "uart.h"
#include "string_buffer.h"

// - defines -------------------------------------------------------------------
#define MOTORA_OCR_U OCR2B
#define MOTORA_OCR_V OCR0B
#define MOTORA_OCR_W OCR0A
#define MOTORB_OCR_U OCR1AL
#define MOTORB_OCR_V OCR1BL
#define MOTORB_OCR_W OCR2A

// - private functions ---------------------------------------------------------
static volatile uint8_t tim_cnt = 0;
#define TIM_CNT_RELOAD 8 // 16 // 32 // 62
// tested: 8 is the fastest!
ISR(TIMER1_COMPA_vect)
{
    if (tim_cnt == 0)
    {
        tim_cnt = TIM_CNT_RELOAD;
        SEND_EVENT(EV_UPDATE_PWM);
    }
    else
    {
        tim_cnt--;
    }
}

// - public funcitons ----------------------------------------------------------

// pwm u: PD1/OC2B
// pwm v: PD5/OC0B
// pwm w: PD6/OC0A
// f_pwm = F_CPU / prescaler / (TOP+1)
// f_pwm = 16000000 Hz / 1 / (255+1) = 62500 Hz (above audible freq)
void motor_start(void)
{
    // INT
    TIMSK1 = 0;
    // gpios
    DDR_MOTORA |= ((1 << PIN_MOTORA_U) | (1 << PIN_MOTORA_V) | (1 << PIN_MOTORA_W));
    PORT_MOTORA &= ~((1 << PIN_MOTORA_U) | (1 << PIN_MOTORA_V) | (1 << PIN_MOTORA_W));
    DDR_MOTORB |= ((1 << PIN_MOTORB_U) | (1 << PIN_MOTORB_V) | (1 << PIN_MOTORB_W));
    PORT_MOTORB &= ~((1 << PIN_MOTORB_U) | (1 << PIN_MOTORB_V) | (1 << PIN_MOTORB_W));
    // set OCR = 0 -> output low
    MOTORA_OCR_U = 0;
    MOTORA_OCR_V = 0;
    MOTORA_OCR_W = 0;
    MOTORB_OCR_U = 0;
    MOTORB_OCR_V = 0;
    MOTORB_OCR_W = 0;
    // set output config
    TCCR0A = (1 << COM0A1) | (1 << COM0A0) | (1 << COM0B1) | (1 << COM0B0) | (1 << WGM01) | (1 << WGM00);
    TCCR1A = (1 << COM0A1) | (1 << COM0A0) | (1 << COM0B1) | (1 << COM0B0) | (1 << WGM10);
    TCCR1C = 0;
    ICR1H = 0;
    ICR1L = 255; // TOP
    TCCR2A = (1 << COM2A1) | (1 << COM2A0) | (1 << COM2B1) | (1 << COM2B0) | (1 << WGM21) | (1 << WGM20);
    // run using prescaler = 1
    TCCR0B = (1 << CS00);
    // do not start yet
    TCCR1B = (1 << WGM12) | (1 << CS10);
    TCCR2B = (1 << CS20);

    uart_puts("motor_start()\n");
    // enable interrupt
    tim_cnt = TIM_CNT_RELOAD;
    TIFR1 |= (1 << OCF1A);
    TIMSK1 |= (1 << OCIE1A);
}

void motorA_update_pwm(uint8_t pwm_u, uint8_t pwm_v, uint8_t pwm_w)
{
    MOTORA_OCR_U = pwm_u;
    MOTORA_OCR_V = pwm_v;
    MOTORA_OCR_W = pwm_w;
    /*
        string_buffer_new();
        string_buffer_append_uint8(pwm_u);
        string_buffer_append_separator();
        string_buffer_append_uint8(pwm_v);
        string_buffer_append_separator();
        string_buffer_append_uint8(pwm_w);
        string_buffer_append_new_line();
        string_buffer_send_uart();*/
}

void motorB_update_pwm(uint8_t pwm_u, uint8_t pwm_v, uint8_t pwm_w)
{
    MOTORB_OCR_U = pwm_u;
    MOTORB_OCR_V = pwm_v;
    MOTORB_OCR_W = pwm_w;
    /*
        string_buffer_new();
        string_buffer_append_uint8(pwm_u);
        string_buffer_append_separator();
        string_buffer_append_uint8(pwm_v);
        string_buffer_append_separator();
        string_buffer_append_uint8(pwm_w);
        string_buffer_append_new_line();
        string_buffer_send_uart();*/
}

void motor_stop(void)
{
    TIMSK1 = 0;
    TCCR0B = 0;
    TCCR1B = 0;
    TCCR2B = 0;
}
