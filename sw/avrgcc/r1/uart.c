/**
 * Martin Egli
 * 2021-07-16
 * http://actrl.cz/blog/index.php/2016/arduino-nano-ch340-schematics-and-details/
 * https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
 * http://asciitable.com/
 *
 * avr-gcc see: /usr/lib/avr
 */

/* - includes --------------------------------------------------------------- */
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

/* - defines ---------------------------------------------------------------- */
#define GPIO_RX _BV(0)
#define GPIO_TX _BV(1)
/* - public variables ------------------------------------------------------- */

/* - functions -------------------------------------------------------------- */

void uart_Init(uint8_t ubrr_val) {
    UBRR0 = ubrr_val;
    UCSR0B |= (1<<TXEN0);
    // Frame Format: Asynchron 8N1
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

/**
 * put char
 * @param   c char
 */
void uart_putc(char c) {
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}

/**
 * put string
 * @param   s string
 */
void uart_puts (char *s) {
    while (*s) {
        // until '\0'
        if(*s == '\n') {
            uart_putc(0x0D); // CR
            uart_putc(0x0A); // LF
            s++;
        }
        else {
            uart_putc(*s++);
        }
    }
}
