/**
 * Martin Egli
 * 2023-09-02
 * mmlib: basic uart functions without hal
 *   these functions actually do not work as they lack hal, they are intended
 *   as a basic structure.
 */

#ifndef _MM_UART_H_
#define _MM_UART_H_

// - use c standard libraries --------------------------------------------------
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "fifo.h"

#define BAUDRATE_9600 (0)
#define BAUDRATE_57600 (1)
#define BAUDRATE_115200 (2)

// - typedefs ------------------------------------------------------------------

typedef struct {
    uint16_t baudrate;  // 
    fifo_t rx_fifo;    // fifo to receive data, pointer to external fifo
    fifo_t tx_fifo;    // fifo to send data, pointer to private (static) fifo, do not change in any way!
} uart_ctrl_t;

// - public functions ----------------------------------------------------------

/**
 * initialize uart, give baud rate
 * @param   uart_ctrl   uart ctrl struct
 * @param   baud        baudrate to set
 * @return  =true: success, =false: error
 */
//uint16_t uart_init(uart_ctrl_t *uart_ctrl, uint16_t baud);
uint16_t uart_init(void);

/**
 * set tx fifo
 * @param   uart_ctrl   uart ctrl struct
 * @param   tx_data     pointer to tx_data
 * @param   tx_size     size of tx_data
 * @return  =true: success, =false: error
 */
//uint16_t uart_set_tx_fifo(uart_ctrl_t *uart_ctrl, uint8_t *tx_data, uint16_t tx_size);

/**
 * set tx fifo
 * @param   uart_ctrl   uart ctrl struct
 * @param   rx_data     pointer to tx_data
 * @param   rx_size     size of tx_data
 * @return  =true: success, =false: error
 */
//uint16_t uart_set_rx_fifo(uart_ctrl_t *uart_ctrl, uint8_t *rx_data, uint16_t rx_size);

/**
 * enable the uart by this uart_ctrl
 * @param   uart_ctrl   uart ctrl struct
 * @return  =true: success, =false: error
 */
//uint16_t uart_enable(uart_ctrl_t *uart_ctrl);

/**
 * enable the uart by this uart_ctrl
 * @param   uart_ctrl   uart ctrl struct
 * @return  =true: success, =false: error
 */
//uint16_t uart_disable(uart_ctrl_t *uart_ctrl);

/**
 * send data from a buffer over uart
 * @param   uart_ctrl   uart ctrl struct
 * @return  nb bytes copied from buffer to tx_fifo
 */
//uint16_t uart_send_buffer(uart_ctrl_t *uart_ctrl, uint8_t *buffer, uint16_t length);
uint16_t uart_send_buffer(uint8_t *buffer, uint16_t length);

/**
 * send a string '\0' terminated over uart, '\0' will not be sent
 * @param   uart_ctrl   uart ctrl struct
 * @return  nb bytes copied from buffer to tx_fifo
 */
//uint16_t uart_send_string(uart_ctrl_t *uart_ctrl, char *str);
uint16_t uart_send_string(char *str);

/**
 * start receiving
 * @param   uart_ctrl   uart ctrl struct
 * @return  =true: success, =false: error
 */
//uint16_t uart_start_recieve(uart_ctrl_t *uart_ctrl);

/**
 * stop receiving
 * @param   uart_ctrl   uart ctrl struct
 * @return  =true: success, =false: error
 */
//uint16_t uart_stop_recieve(uart_ctrl_t *uart_ctrl);

#endif // _MM_UART_H_
