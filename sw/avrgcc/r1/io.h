/**
 * Martin Egli
 * 2022-06-06
 *
 * define all ios for this project
 */

#ifndef _IO_H_
#define _IO_H_
/* - includes --------------------------------------------------------------- */

// Motor A PWM, PORTD
#define DDR_MOTORA (DDRD)
#define PORT_MOTORA (PORTD)
#define PIN_MOTORA_U (1) // PD1/OC2B: motor a, pwm u
#define PIN_MOTORA_V (5) // PD5/OC0B: motor a, pwm v
#define PIN_MOTORA_W (6) // PD6/OC0A: motor a, pwm w

// ISP PORTB: 3, 4, 5

// XTAL, PORTB
#define PIN_XTAL1 (6)
#define PIN_XTAL2 (7)

// rotary encoder PORTC
#define PIN_WHEEL_A (0) // A0, PCINT8, rotary encoder signal A
#define PIN_WHEEL_B (1) // A1, PCINT9, rotary encoder signal B

// OLED I2C, PORTC
#define PIN_OLED_SDA (4) // A5
#define PIN_OLED_SCL (5) // A4

// UART PORTD
#define PIN_RXD (0)
#define PIN_TXD (1)

// Buttons PORTD
#define PIN_BUTTON_ON_OFF (2) // D2, INT0, PCINT18,  on/off button
#define PIN_BUTTON_WHEEL (3)  // D3, INT1, PCINT19,  button of rotary encoder

// fixed PWM, PORTD
#define PIN_OC0A (6) // D6, fixed 1 MHz 50% pwm signal, also (on/off)

#endif /* _IO_H_ */