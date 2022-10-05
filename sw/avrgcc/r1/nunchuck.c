/**
 * Martin Egli
 * 2022-09-26
 * Nunchuck module, using i2c
 * see https://create.arduino.cc/projecthub/infusion/using-a-wii-nunchuk-with-arduino-597254
 */

// - includes ------------------------------------------------------------------
#include "project.h"
#include "nunchuck.h"
#include "i2c.h"

// - defines -------------------------------------------------------------------
static const nunchuck_dev_addr = 0x52 << 1;
static uint8_t nunchuck_data[16];
static uint8_t nunchuch_data_length;
static uint8_t nunchuck_write_reg(uint8_t length)
{
    if ((nunchuch_data_length = i2c_write_buffer(nunchuck_dev_addr, nunchuck_data, length)) != length)
    {
        return RET_ERR;
    }
    return RET_OK;
}

static uint8_t nunchuck_read_reg(uint8_t reg_addr, uint8_t length)
{
    if (i2c_write_reg_addr(nunchuck_dev_addr, reg_addr) != 1)
    {
        return RET_ERR;
    }
    if ((nunchuch_data_length = i2c_read_buffer(nunchuck_dev_addr, nunchuck_data, length)) != length)
    {
        return RET_ERR;
    }
    return RET_OK;
}

// - public functions ----------------------------------------------------------

void nunchuck_start_unencrypred(void)
{
    nunchuck_data[0] = 0xF0;
    nunchuck_data[1] = 0x55;
    nunchuck_write_reg(2);
    nunchuck_data[0] = 0xFB;
    nunchuck_data[1] = 0x00;
    nunchuck_write_reg(2);

    // read indent
    nunchuck_read_reg(0xFA, 6);
}

uint8_t nunchuck_read_raw(uint8_t *buffer, uint8_t length)
{
    uint8_t n;
    if (nunchuck_read_reg(0x00, 6) == RET_ERR)
    {
        return 0;
    }
    for (n = 0; n < length; n++)
    {
        buffer[n] = nunchuck_data[n];
    }
    return n;
}
