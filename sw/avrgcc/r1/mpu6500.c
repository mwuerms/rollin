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

static const mpu650_dev_addr = 0x68 << 1;

#define MPU650_DATA_BUFFER_SIZE (16)
static uint8_t mpu6500_data_buffer[MPU650_DATA_BUFFER_SIZE];
static uint8_t mpu6500_data_length;
static uint8_t mpu650_write_reg(uint8_t length)
{
    if ((mpu6500_data_length = i2c_write_buffer(mpu650_dev_addr, mpu6500_data_buffer, length)) != length)
    {
        return RET_ERR;
    }
    return RET_OK;
}

static uint8_t mpu650_read_reg(uint8_t reg_addr, uint8_t length)
{
    if (i2c_write_reg_addr(mpu650_dev_addr, reg_addr) != 1)
    {
        return RET_ERR;
    }
    if ((mpu6500_data_length = i2c_read_buffer(mpu650_dev_addr, mpu6500_data_buffer, length)) != length)
    {
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

uint8_t mpu6500_init(void)
{
    uint8_t n;

    // DDRC |= (_BV(4) | _BV(5));
    // PORTC |= (_BV(4) | _BV(5));
    i2c_init();

    uart_puts("mpu6500_init\n  who am i: 0x");
    if (mpu650_read_reg(117, 1) == RET_ERR)
    {
        // signal: could not read
        uart_puts("error: could not read\n");
    }
    else
    {
        string_buffer_new();
        string_buffer_append_uint8_hex(mpu6500_data_buffer[0]);
        string_buffer_append_new_line();
        string_buffer_send_uart();
    }
    /*
        // read config
        uart_puts("config after reset:\n");
        uart_puts("reg (hex), content (hex)\n");
        if (mpu650_read_reg(25, 12) == RET_ERR)
        {
            // signal: could not read
            uart_puts("\nerror: could not read\n");
        }
        else
        {
            for (n = 0; n < 12; n++)
            {
                string_buffer_new();
                string_buffer_append_uint8_hex(n + 25);
                string_buffer_append_char(',');
                string_buffer_append_uint8_hex(mpu6500_data_buffer[n]);
                string_buffer_append_new_line();
                string_buffer_send_uart();
            }
        }
        // read data
        uart_puts("read data\n");
        uart_puts("reg (hex), content (hex)\n");
        if (mpu650_read_reg(59, 14) == RET_ERR)
        {
            // signal: could not read
            uart_puts("\nerror: could not read\n");
        }
        else
        {
            for (n = 0; n < 14; n++)
            {
                string_buffer_new();
                string_buffer_append_uint8_hex(n + 59);
                string_buffer_append_char(',');
                string_buffer_append_uint8_hex(mpu6500_data_buffer[n]);
                string_buffer_append_new_line();
                string_buffer_send_uart();
            }
        }*/
}

uint8_t mpu6500_set_config(void)
{
    mpu6500_data_buffer[0] = 26;   // reg_addr
    mpu6500_data_buffer[1] = 0x00; // 26
    mpu6500_data_buffer[2] = 0x00; // 27: gyro config: +/-250 dps, 250 Hz bandwidth, 0.97 ms delay
    mpu6500_data_buffer[3] = 0x00; // 28: accel config: +/- 2 g
    mpu6500_data_buffer[4] = 0x00; // 29: accel config 2: 460 Hz bandwidth, 1.94 ms delay
    mpu6500_data_buffer[5] = 0x00; // 30: low power acc odr ctrl: 0.24 Hz
    mpu6500_data_buffer[6] = 0x00; // 31
    if (mpu650_write_reg(7) == RET_ERR)
    {
        return RET_ERR;
    }
    return RET_OK;
    /*mpu6500_data_buffer[0] = 106;  // reg_addr
    mpu6500_data_buffer[1] = 0x00; // 106: user ctrl
    mpu6500_data_buffer[2] = 0x00; // 27: gyro config: +/-250 dps, 250 Hz bandwidth, 0.97 ms delay
    mpu6500_data_buffer[3] = 0x00; // 28: accel config: +/- 2 g
    mpu6500_data_buffer[4] = 0x00; // 29: accel config 2: 460 Hz bandwidth, 1.94 ms delay
    mpu6500_data_buffer[5] = 0x00; // 30: low power acc odr ctrl: 0.24 Hz
    mpu6500_data_buffer[6] = 0x00; // 31
    if (mpu650_write_reg(7) == RET_ERR)
    {
        return RET_ERR;
    }
    return RET_OK;*/
}

uint8_t mpu6500_read_sensor_data(uint8_t *buffer, uint8_t length)
{
    uint8_t n;
    if (mpu650_read_reg(59, 14) == RET_ERR)
    {
        return RET_ERR;
    }
    for (n = 0; n < length; n++)
    {
        buffer[n] = mpu6500_data_buffer[n];
    }
    return RET_OK;
}
