/**
 * Martin Egli
 * 2025-06-30
 * angle sensor module
 */

#include "angle_sensor.h"

// - i2c hal mockups -----------------------------------------------------------
// to be replaced
inline uint16_t i2c_wr_blocking(angle_sens_t *as) {
    // tx i2c_addr + WR
    if(as == NULL) {
        // error, invalid sensor
        return false;
    }
    return 0;
}

inline uint16_t i2c_rd_blocking(angle_sens_t *as) {
    // tx i2c_addr + RD
    if(as == NULL) {
        // error, invalid sensor
        return false;
    }
    return 0;
}

// - public functions ----------------------------------------------------------
uint16_t angle_sensor_init(angle_sens_t *as, I2C_TypeDef *hi2c) {
    if(as == NULL) {
        // error, invalid sensor
        return false;
    }
    if(hi2c == NULL) {
        // error, invalid i2c device
        return false;
    }
    as->type = ANGLE_SENSOR_TYPE_AS5600;
    as->i2c.addr = AS5600_I2C_ADDR;
    return true;
}

uint16_t angle_sensor_get(angle_sens_t *as) {
    uint8_t buf[8];
    if(as == NULL) {
        // error, invalid sensor
        return false;
    }
    as->i2c.buf[0] = 0x0C;
    as->i2c.buf_len = 1;
    i2c_wr_blocking(as);
    i2c_rd_blocking(as);
    as->raw_angle = ((uint16_t*)&buf[0])[0];
    as->angle_deg = (360 * as->raw_angle)/4096;
    return true;
}
