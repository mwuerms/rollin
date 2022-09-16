/**
 * Martin Egli
 * 2022-09-13
 * com module
 */

#ifndef _COM_H_
#define _COM_H_

// - include ------------------------------------------------------------------------
#include <stdint.h>

// - public functions ---------------------------------------------------------------
uint16_t com_exec(uint8_t *packet, uint16_t length);

#endif // _COM_H_
