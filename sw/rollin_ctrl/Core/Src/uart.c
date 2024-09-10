/**
 * Martin Egli
 * 2023-09-02
 * mmlib: basic uart functions without hal
 *   these functions actually do not work as they lack hal, they are intended
 *   as a basic structure.
 */

#include <string.h>
#include "uart.h"

// - private functions ---------------------------------------------------------

// - public functions ----------------------------------------------------------

uint16_t uart_init(uart_ctrl_t *uart_ctrl, uint16_t baud) {
    uart_ctrl->baudrate = baud;
    return true;
}

uint16_t uart_set_tx_fifo(uart_ctrl_t *uart_ctrl, uint8_t *tx_data, uint16_t tx_size) {
    fifo_init(&(uart_ctrl->tx_fifo), tx_data, tx_size);
    return true;
}

uint16_t uart_set_rx_fifo(uart_ctrl_t *uart_ctrl, uint8_t *rx_data, uint16_t rx_size) {
    fifo_init(&(uart_ctrl->tx_fifo), rx_data, rx_size);
    return true;
}

uint16_t uart_enable(uart_ctrl_t *uart_ctrl) {
    return true;
}

uint16_t uart_disable(uart_ctrl_t *uart_ctrl) {
    return true;
}

uint16_t uart_send_buffer(uart_ctrl_t *uart_ctrl, uint8_t *buffer, uint16_t length) {
    uint16_t n;
    for(n = 0; n < length; n++) {
        if(fifo_try_append(&(uart_ctrl->tx_fifo)) == false) {
            // fifo is full, stop here
            break;
        }
        ((uint8_t *)(uart_ctrl->tx_fifo.data))[uart_ctrl->tx_fifo.wr_proc] = buffer[n];
        fifo_finalize_append(&(uart_ctrl->tx_fifo));
    }

    if(fifo_is_empty(&(uart_ctrl->tx_fifo)) == false) {
        // fifo is not empty, start transmitting now
    }
    return n;
}

uint16_t uart_send_string(uart_ctrl_t *uart_ctrl, char *str) {
    uint16_t n, len;
    len = strlen(str);
    for(n = 0;n < len;n++) {
        if(str[n] == 0) {
            // end of string found, stop here
            break;
        }
        if(fifo_try_append(&(uart_ctrl->tx_fifo)) == false) {
            // fifo is full, stop here
            break;
        }
        ((uint8_t *)(uart_ctrl->tx_fifo.data))[uart_ctrl->tx_fifo.wr_proc] = str[n];
        fifo_finalize_append(&(uart_ctrl->tx_fifo));
    }

    if(fifo_is_empty(&(uart_ctrl->tx_fifo)) == false) {
        // fifo is not empty, start transmitting now
    }
    return n;
}

uint16_t uart_start_recieve(uart_ctrl_t *uart_ctrl) {
    return true;
}

uint16_t uart_stop_recieve(uart_ctrl_t *uart_ctrl) {
    return true;
}
