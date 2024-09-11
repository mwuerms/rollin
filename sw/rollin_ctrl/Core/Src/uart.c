/**
 * Martin Egli
 * 2023-09-02
 * mmlib: basic uart functions without hal
 *   these functions actually do not work as they lack hal, they are intended
 *   as a basic structure.
 */

#include <string.h>
#include "uart.h"
#include "usart.h"

// - private definitions -------------------------------------------------------
#define UART_DEVICE USART1

// - private variables -------------------------------------------------------
static uart_ctrl_t *uart_ctrl_ptr = NULL;
#define UART_BUFFER_SIZE	(64)
static uint8_t uart_tx_buffer[UART_BUFFER_SIZE];

// - private functions ---------------------------------------------------------

// called in void USART1_IRQHandler(void)
void uart_irq_handler(void) {
	uint8_t c;
	if (LL_USART_IsActiveFlag_TXE(UART_DEVICE)) {
		// TX Empty -> write to TX
		if(fifo_try_get(&(uart_ctrl_ptr->tx_fifo)) == true) {
			c = ((uint8_t *)(uart_ctrl_ptr->tx_fifo.data))[uart_ctrl_ptr->tx_fifo.rd_proc];
			LL_USART_TransmitData8(UART_DEVICE, c);
			fifo_finalize_get(&(uart_ctrl_ptr->tx_fifo));
		}
		else {
			// no more data to send -> clear TX Empty
			LL_USART_DisableIT_TXE(UART_DEVICE);
		}
	}
}

static void uart_start_transmit(uart_ctrl_t *uart_ctrl) {
	if(fifo_is_empty(&(uart_ctrl->tx_fifo)) == true) {
		// error, fifo is empty
		return;
	}
	// start transmit -> set TX Empty
	LL_USART_EnableIT_TXE(UART_DEVICE);
}

// - public functions ----------------------------------------------------------

uint16_t uart_init(uart_ctrl_t *uart_ctrl, uint16_t baud) {
    uart_ctrl->baudrate = baud;

    MX_USART1_UART_Init();

    uart_ctrl_ptr = uart_ctrl; // save uart_ctrl
    uart_set_tx_fifo(uart_ctrl, uart_tx_buffer, UART_BUFFER_SIZE);
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
    uart_start_transmit(uart_ctrl);
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

    uart_start_transmit(uart_ctrl);
    return n;
}

uint16_t uart_start_recieve(uart_ctrl_t *uart_ctrl) {
    return true;
}

uint16_t uart_stop_recieve(uart_ctrl_t *uart_ctrl) {
    return true;
}
