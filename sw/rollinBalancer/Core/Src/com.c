/**
 * Martin Egli
 * 2022-09-13
 * com module
 */

// - include ------------------------------------------------------------------------
#include "com.h"

// - private functions --------------------------------------------------------------

static uint16_t _f_unused(uint8_t *packet, uint16_t length) {
	return 0;
}

static uint16_t _f0(uint8_t *packet, uint16_t length) {
	return 0;
}

// - function table -----------------------------------------------------------------
static uint16_t (* const _FuncTable[16])(void)=
{
	_f0,	// 0
	_f_unused,	// 1
	_f_unused,	// 2
	_f_unused,	// 3
	_f_unused,	// 4
	_f_unused,	// 5
	_f_unused,	// 6
	_f_unused,	// 7
	_f_unused,	// 8
	_f_unused,	// 9
	_f_unused,	// 10
	_f_unused,	// 11
	_f_unused,	// 12
	_f_unused,	// 13
	_f_unused,	// 14
	_f_unused	// 15
};
// - public functions ---------------------------------------------------------------
// func, length, data
uint16_t com_exec(uint8_t *packet, uint16_t length) {
	return 0;
}

