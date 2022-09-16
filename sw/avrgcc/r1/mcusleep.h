/**
 * Martin Egli
 * 2022-06-06
 *
 */

#ifndef _MCU_SLEEP_H_
#define _MCU_SLEEP_H_
/* - includes --------------------------------------------------------------- */

void sleep_mode_init(void);
void enter_sleep_mode(void);

// use -fshort-enums for enums
enum sleep_modes{
    ACTIVE = 0,
    IDLE,
    POWER_DOWN,
    POWER_SAVE,
    STANDBY,
};
void use_sleep_mode(enum sleep_modes mode);
void release_sleep_mode(enum sleep_modes mode);

#endif /* _MCU_SLEEP_H_ */
