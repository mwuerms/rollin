/**
 * Martin Egli
 * 2022-06-10
 *
 */

#ifndef _WDT_H_
#define _WDT_H_

/* - includes --------------------------------------------------------------- */
#include <stdint.h>

// - public functions ----------------------------------------------------------
#define cEV_TIMER_INTERVAL_16MS     (0)
#define cEV_TIMER_INTERVAL_32MS     (1)
#define cEV_TIMER_INTERVAL_64MS     (2)
#define cEV_TIMER_INTERVAL_125MS    (3)
#define cEV_TIMER_INTERVAL_250MS    (4)
#define cEV_TIMER_INTERVAL_500MS    (5)
#define cEV_TIMER_INTERVAL_1S       (6)
#define cEV_TIMER_INTERVAL_2S       (7)
void wdtTimer_StartTimeout(uint8_t timeout, uint8_t interval,  uint8_t global_event_flag, uint8_t detail_event_flag);
void wdtTimer_Stop(void);

#endif // _WDT_H_
