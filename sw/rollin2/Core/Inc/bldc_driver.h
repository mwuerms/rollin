/**
 * Martin Egli
 * 2025-06-29
 * bldc driver module
 */

#ifndef _BLDC_DRIVER_H_ 
#define _BLDC_DRIVER_H_ 

#include <stdint.h>
#include <stdbool.h>
#include "project.h"
#include "tim.h"

typedef struct {
	TIM_TypeDef *tim;
    uint16_t channels;
    uint16_t state;
} bldc_driver_t;

uint16_t bldc_driver_init(bldc_driver_t *d, TIM_TypeDef *tim);
uint16_t bldc_driver_enable(bldc_driver_t *d);
uint16_t bldc_driver_disable(bldc_driver_t *d);
uint16_t bldc_driver_spwm(bldc_driver_t *d);

#endif // _BLDC_DRIVER_H_
