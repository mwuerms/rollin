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
#include <util/atomic.h>

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

typedef struct
{
    uint8_t u, v, w;
} motor_index_t;

// - private variables ---------------------------------------------------------
// static uint8_t sine_lookup[] = {127, 135, 143, 151, 159, 167, 175, 183, 190, 197, 204, 210, 216, 222, 227, 232, 236, 240, 244, 247, 249, 251, 252, 253, 254, 253, 252, 251, 249, 247, 244, 240, 236, 232, 227, 222, 216, 210, 204, 197, 190, 183, 175, 167, 159, 151, 143, 135, 127, 118, 110, 102, 94, 86, 78, 70, 63, 56, 49, 43, 37, 31, 26, 21, 17, 13, 9, 6, 4, 2, 1, 0, 0, 0, 1, 2, 4, 6, 9, 13, 17, 21, 26, 31, 37, 43, 49, 56, 63, 70, 78, 86, 94, 102, 110, 118};
// static motor_index_t pwm_index_a = {.u = 0, .v = 32, .w = 64};
static uint8_t sine_lookup[] = {127, 143, 159, 175, 190, 204, 216, 227, 236, 244, 249, 252, 254, 252, 249, 244, 236, 227, 216, 204, 190, 175, 159, 143, 127, 110, 94, 78, 63, 49, 37, 26, 17, 9, 4, 1, 0, 1, 4, 9, 17, 26, 37, 49, 63, 78, 94, 110};
static motor_index_t pwm_index_a = {.u = 0, .v = 16, .w = 32};
// static uint8_t sine_lookup[] = {127, 159, 190, 216, 236, 249, 254, 249, 236, 216, 190, 159, 127, 94, 63, 37, 17, 4, 0, 4, 17, 37, 63, 94};
// static motor_index_t pwm_index_a = {.u = 0, .v = 8, .w = 16};
// static uint8_t sine_lookup[] = {127, 190, 236, 254, 236, 190, 127, 63, 17, 0, 17, 63};
// static motor_index_t pwm_index_a = {.u = 0, .v = 4, .w = 8};

static const uint8_t nb_sine_steps = NB_ELEMENTS(sine_lookup);

// - private functions ---------------------------------------------------------

#define INC(x)                            \
    do                                    \
    {                                     \
        if (x < NB_ELEMENTS(sine_lookup)) \
            x++;                          \
        else                              \
            x = 0;                        \
    } while (0)

#define DEC(x)                            \
    do                                    \
    {                                     \
        if (x == 0)                       \
            x = NB_ELEMENTS(sine_lookup); \
        else                              \
            x--;                          \
    } while (0)

static uint8_t inc_uint8_max(uint8_t value, uint8_t max)
{
    if (value < (max - 1))
    {
        return value + 1;
    }
    return 0;
}

static uint8_t dec_uint8_reload(uint8_t value, uint8_t reload)
{
    if (value)
    {
        return value - 1;
    }
    return reload;
}

static volatile uint8_t tim_cnt = 0;
#define TIM_CNT_RELOAD 32 // all OK: 8 // 16 // 32 // 62
// tested: 8 is the fastest!

static volatile uint8_t speed_a_cnt, speed_a;
#define SPEED_A_MAX 16

// factor 256 = 1
#define PWM_SCALE (0.6666667 * 256)
static inline uint8_t scale(uint8_t value, uint8_t factor)
{
    uint16_t calc = factor * value;
    return ((uint8_t *)&calc)[1];
}

static volatile uint8_t motor_move_dir = 0; // 1: foreward: INC, 0: backwards: DEC
static inline move_motor(void)
{
    if (motor_move_dir == 0)
    {
        // backwards
        DEC(pwm_index_a.u);
        DEC(pwm_index_a.v);
        DEC(pwm_index_a.w);
    }
    else
    {
        // forewards
        INC(pwm_index_a.u);
        INC(pwm_index_a.v);
        INC(pwm_index_a.w);
    }
}

static inline void set_motor_a(void)
{
    MOTORA_OCR_U = scale(sine_lookup[pwm_index_a.u], PWM_SCALE);
    MOTORA_OCR_V = scale(sine_lookup[pwm_index_a.v], PWM_SCALE);
    MOTORA_OCR_W = scale(sine_lookup[pwm_index_a.w], PWM_SCALE);
}

static inline void set_motor_b(void)
{
    // use same values
    // INT(pwm_index_a.u);
    // INT(pwm_index_a.v);
    // INT(pwm_index_a.w);
    MOTORB_OCR_U = scale(sine_lookup[pwm_index_a.u], PWM_SCALE);
    MOTORB_OCR_V = scale(sine_lookup[pwm_index_a.v], PWM_SCALE);
    MOTORB_OCR_W = scale(sine_lookup[pwm_index_a.w], PWM_SCALE);
}

ISR(TIMER1_OVF_vect)
{
    if (tim_cnt == 0)
    {
        tim_cnt = TIM_CNT_RELOAD;
        // SEND_EVENT(EV_UPDATE_PWM);

        if (speed_a_cnt == 0)
        {
            speed_a_cnt = SPEED_A_MAX;
        }
        else
        {
            speed_a_cnt--;
        }
        if (speed_a_cnt < speed_a)
        {
            move_motor();
            set_motor_a();
            set_motor_b();
        }
    }
    else
    {
        tim_cnt--;
    }
}

static volatile uint8_t tim0_event_to_send;
static volatile uint16_t tim0_ticks_cnt_dwn;
ISR(TIMER0_OVF_vect)
{
    if (tim0_ticks_cnt_dwn == 0)
    {
        if (tim0_event_to_send)
        {
            TIMSK0 = 0;
            SEND_EVENT(tim0_event_to_send);
            tim0_event_to_send = 0;
        }
    }
    else
    {
        tim0_ticks_cnt_dwn--;
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
    // TIMER1_COMPA_vect
    // TIFR1 |= (1 << OCF1A);
    // TIMSK1 |= (1 << OCIE1A);
    // TIMER1_OVF_vect
    TIFR1 |= (1 << TOV1);
    TIMSK1 |= (1 << TOIE1);
}

// f = 62500 Hz
void motor_send_event_after_nb_ticks(uint16_t nb_ticks, uint8_t event)
{
    TIMSK0 = 0;

    tim0_ticks_cnt_dwn = nb_ticks;
    tim0_event_to_send = event;

    // TIMER0_OVF_vect
    TIFR0 |= (1 << TOV0);
    TIMSK0 |= (1 << TOIE0);
}

void motor_set_speed(int8_t set_speed)
{
    uint8_t sreg = SREG;
    cli();

    speed_a_cnt = SPEED_A_MAX;
    if (set_speed < 0)
    {
        // backwards
        motor_move_dir = 0;
        set_speed *= -1;
    }
    else
    {
        // forewards
        motor_move_dir = 1;
    }
    speed_a = set_speed;

    SREG = sreg;
}

void motor_stop(void)
{
    TIMSK1 = 0;
    TCCR0B = 0;
    TCCR1B = 0;
    TCCR2B = 0;
}
