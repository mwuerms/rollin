/**
 * Martin Egli
 * 2021-07-16
 *
 * use uart module
 */
#ifndef _UART_H_
#define _UART_H_

/* - includes --------------------------------------------------------------- */
#include "project.h"
#include <stdint.h>

/* - public variables ------------------------------------------------------- */

/* - functions -------------------------------------------------------------- */

#define UBRR_VAL_9600       103 //    9600: 104.16-1
#define UBRR_VAL_57600      16  //   57600:  17.36-1
#define UBRR_VAL_115200     6   //  115200:   7.68-1
#define UBRR_VAL_230400     2   //  230400:   3.34-1
#define UBRR_VAL_1000000    0   // 1000000:   1.00-1
void uart_Init(uint8_t ubrr_val);

void uart_putc(char c);
void uart_puts(char *s);
#endif /* _UART_H_ */
