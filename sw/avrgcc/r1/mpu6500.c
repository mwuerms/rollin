/**
 * Martin Egli
 * 2021-09-16
 *
 * mpu6500 module
 */

// - includes ------------------------------------------------------------------
#include "project.h"
#include <stdint.h>

#include "i2c.h"
#include "string_buffer.h"

static const mpu650_addr = 0x68;

static uint8_t who = 0;
static uint8_t _read(uint8_t dev_addr, uint8_t reg_addr) {
    if(i2c_write_reg_addr(dev_addr, 117) != 1) {
        return 0;
    }
    if(i2c_read_buffer(dev_addr, &who, 1) != 1) {
        return 0;
    }
    return 1;
}

static uint8_t _find(void) {
    uint8_t dev_addr = 0x10;

    uart_puts("_find()\n addr:\n");
    for(dev_addr = 0x00; dev_addr < 0xF0; dev_addr += 2) {
        // READ = 1
        string_buffer_new();
        string_buffer_append_uint8(dev_addr);
        string_buffer_send_uart();
        if(_read(dev_addr, 117)) {
            uart_puts(" found, who: ");
            string_buffer_new();
            string_buffer_append_uint8(who);
            string_buffer_send_uart();
        }
        uart_putc('\n');
    }
}

// - functions -----------------------------------------------------------------

uint8_t mpu6500_init(void) {
    i2c_init();
    uart_puts("mpu6500_init\n  find i2c addr");

    _find();
    
}
