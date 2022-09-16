/**
 * Martin Egli
 * 2022-06-06
 *
 */

// - includes ------------------------------------------------------------------
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>

#include "project.h"
#include "mcusleep.h"
#include "macrolib.h"

// - private variables ---------------------------------------------------------

static struct {
    uint8_t active;
    uint8_t idle;
    uint8_t power_down;
    uint8_t power_save;
    uint8_t standby;
} sleep_mode_ctrl;

/**
 * enter the given sleep mode, see avr/sleep.h
 * @param   mode    sleep modes:
 * translate: own defines to arv/sleep.h:
 * cSLEEP_MODE_... | SLEEP_MODE_...
 * cSLEEP_MODE_ACTIVE | none, do not enter this function
 * cSLEEP_MODE_IDLE | SLEEP_MODE_IDLE
 *  none | SLEEP_MODE_ADC
 * cSLEEP_MODE_POWER_DOWN | SLEEP_MODE_PWR_DOWN
 *  none | SLEEP_MODE_PWR_SAVE
 *  none | SLEEP_MODE_STANDBY
 *  none | SLEEP_MODE_EXT_STANDBY
 */
/*
static void _EnterSleepMode(uint8_t mode) {
    set_sleep_mode(mode);
    sleep_enable();
    sei();
    sleep_cpu();    // enter sleep mode, wait here
    sleep_disable();
    cli();
}*/
//_EnterSleepMode(SLEEP_MODE_PWR_DOWN);
// - public functions ----------------------------------------------------------
void sleep_mode_init(void) {
    memset(&sleep_mode_ctrl, 0, sizeof(sleep_mode_ctrl));
}

void enter_sleep_mode(void) {
    // stay in active mode
    sei();
    while(global_events == 0);
}
#if 0
    if(sleep_mode_ctrl.active) {
        // stay in active mode
        sei();
        while(global_events == 0);
    }
    else if(sleep_mode_ctrl.idle) {
        // enter IDLE mode
    }
    else if(sleep_mode_ctrl.power_down) {
        // enter POWER DOWN mode
        //_EnterSleepMode(SLEEP_MODE_PWR_DOWN);
    }
    else if(sleep_mode_ctrl.power_save) {
        // enter POWER SAVE mode
    }
    else { //if(sleep_mode_ctrl.standby) {
        // enter STANDBY mode
    }
}
#endif
void use_sleep_mode(enum sleep_modes mode) {
    switch(mode) {
        case ACTIVE:
            INC_VAR_MAX(sleep_mode_ctrl.active, 255);
            break;
        case IDLE:
            INC_VAR_MAX(sleep_mode_ctrl.idle, 255);
            break;
        case POWER_DOWN:
            INC_VAR_MAX(sleep_mode_ctrl.power_down, 255);
            break;
        case POWER_SAVE:
            INC_VAR_MAX(sleep_mode_ctrl.power_save, 255);
            break;
        case STANDBY:
            INC_VAR_MAX(sleep_mode_ctrl.standby, 255);
            break;
    }
}

void release_sleep_mode(enum sleep_modes mode) {
    switch(mode) {
        case ACTIVE:
            DEC_VAR(sleep_mode_ctrl.active);
            break;
        case IDLE:
            DEC_VAR(sleep_mode_ctrl.idle);
            break;
        case POWER_DOWN:
            DEC_VAR(sleep_mode_ctrl.power_down);
            break;
        case POWER_SAVE:
            DEC_VAR(sleep_mode_ctrl.power_save);
            break;
        case STANDBY:
            DEC_VAR(sleep_mode_ctrl.standby);
            break;
    }
}
