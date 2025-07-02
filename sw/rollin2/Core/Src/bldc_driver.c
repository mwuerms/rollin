/**
 * Martin Egli
 * 2025-06-29
 * bldc driver module
 * use STM32 LL functions
 */

#include "bldc_driver.h"

uint16_t bldc_driver_init(bldc_driver_t *d, TIM_TypeDef *tim) {
    if(d == NULL) {
        // error, invalid driver
        return false;
    }
    d->tim = tim;
    return true;
}

uint16_t bldc_driver_enable(bldc_driver_t *d) {
    if(d == NULL) {
        // error, invalid driver
        return false;
    }
    LL_TIM_OC_SetMode(d->tim, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_PWM1);
    LL_TIM_OC_SetCompareCH1(d->tim, 0);
	LL_TIM_OC_SetCompareCH2(d->tim, 0);
	LL_TIM_OC_SetCompareCH3(d->tim, 0);
	LL_TIM_EnableAllOutputs(d->tim);
    LL_TIM_CC_EnableChannel(d->tim, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH3);
	LL_TIM_EnableCounter(d->tim);
    return true;
}

uint16_t bldc_driver_disable(bldc_driver_t *d) {
    if(d == NULL) {
        // error, invalid driver
        return false;
    }
    LL_TIM_DisableCounter(d->tim);
    LL_TIM_OC_SetMode(d->tim, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_FORCED_INACTIVE);
    LL_TIM_OC_SetCompareCH1(d->tim, 0);
	LL_TIM_OC_SetCompareCH2(d->tim, 0);
	LL_TIM_OC_SetCompareCH3(d->tim, 0);
    // keep channels + outputs enabled, so they stay low
	LL_TIM_EnableAllOutputs(d->tim);
    LL_TIM_CC_EnableChannel(d->tim, LL_TIM_CHANNEL_CH1|LL_TIM_CHANNEL_CH2|LL_TIM_CHANNEL_CH3);
    return true;
}

uint16_t bldc_driver_set_spwm(bldc_driver_t *d, uint16_t ccr_u, uint16_t ccr_v, uint16_t ccr_w) {
    if(d == NULL) {
        // error, invalid driver
        return false;
    }
    return true;
}
