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

#define MPU650_DATA_BUFFER_SIZE (16)
static struct {
    uint8_t data[MPU650_DATA_BUFFER_SIZE];
    uint8_t length;
} mpu6500_buffer;
static uint8_t mpu650_read_reg(uint8_t reg_addr, uint8_t length) {
    if(i2c_write_reg_addr((mpu650_addr<<1), 117) != 1) {
        return 0;
    }
    if((mpu6500_buffer.length = i2c_read_buffer((mpu650_addr<<1), mpu6500_buffer.data, length)) != length) {
        return RET_ERR;
    }
    return RET_OK;
}

/*static uint8_t _find(void) {
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
}*/

// - functions -----------------------------------------------------------------

uint8_t mpu6500_init(void) {
    i2c_init();
    uart_puts("mpu6500_init\n  who am i: 0x");
    mpu650_read_reg(117, 1);
    string_buffer_new();
    string_buffer_append_uint8_hex(mpu6500_buffer.data[0]);
    string_buffer_append_new_line();
    string_buffer_send_uart();
    
}
