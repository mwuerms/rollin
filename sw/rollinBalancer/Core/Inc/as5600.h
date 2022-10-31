/*
 * as5600.h
 *
 *  Created on: Oct 26, 2022
 *      Author: martin
 */

#ifndef INC_AS5600_H_
#define INC_AS5600_H_

// - include -------------------------------------------------------------------
#include <stdint.h>

// - public functions ----------------------------------------------------------
void as5600_init(void);
void as5600_set_config(void);
void as5600_set_zeropos_maxpos(void);
uint16_t as5600_read_angle(void);
uint16_t as5600_read_status(void);

#endif // INC_AS5600_H_
