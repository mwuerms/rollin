/**
 * Martin Egli
 * 2022-06-10
 *
 */

/* - includes --------------------------------------------------------------- */
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "project.h"
#include "wdt.h"

// - private functions ---------------------------------------------------------
static volatile uint8_t wdt_timeout, wdt_global_event_flag;
/**
 * watchdog ISR
 */
ISR(WDT_vect)
{
    if (wdt_timeout == 0)
    { // so "wdtTimer_StartTimeout(1, ...)" does result in delay of 1 and not 2 wdt periodes
        wdtTimer_Stop();
        SEND_EVENT(wdt_global_event_flag);
    }
    else
    {
        wdt_timeout--;
    }
}

// - public functions ----------------------------------------------------------
/**
 * start the wdtTimer
 * @param   timeout     overall timeout = interval * timeout
 */
void wdtTimer_StartTimeout(uint8_t timeout, uint8_t interval, uint8_t global_event_flag)
{
    uint8_t sr;
    // lock_interrupt(sr);
    wdt_timeout = timeout;
    wdt_global_event_flag = global_event_flag;
    WDTCSR = (1 << WDCE) | (1 << WDIF);
    WDTCSR |= (interval & 0x07);
    WDTCSR |= (1 << WDIE);
    // restore_interrupt(sr);
}

/**
 * stop wdtTimer
 */
void wdtTimer_Stop(void)
{
    WDTCSR = _BV(WDCE);
    WDTCSR = 0;
}
