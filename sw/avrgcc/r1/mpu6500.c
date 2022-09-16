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

static const mpu650_addr = 0x68;

static char mpu_str[16];
static void _to_mpu_str(uint8_t val) {
    uint8_t c;
    c = val / 100;
    mpu_str[0] = '0' + c;
    val = val - c * 100;
    
    c = val / 10;
    mpu_str[1] = '0' + c;
    val = val - c * 10;

    mpu_str[2] = '0' + val;
    
    mpu_str[3] = 0; // term
}

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
        _to_mpu_str(dev_addr);
        uart_puts(mpu_str);
        if(_read(dev_addr, 117)) {
            uart_puts(" found, who: ");
            _to_mpu_str(who);
            uart_puts(mpu_str);
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
