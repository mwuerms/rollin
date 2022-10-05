/**
 * Martin Egli
 * 2022-06-06
 *
 */

#ifndef _MACROS_H_
#define _MACROS_H_
/* - includes --------------------------------------------------------------- */
#include <stdint.h>

// - public MACROS -------------------------------------------------------------
#ifndef _BV(bit)
#define _BV(bit) (1 << (bit))
#endif

#define INC_VAR_MAX(var, max) \
    do                        \
    {                         \
        if (var < max)        \
        {                     \
            var++;            \
        }                     \
    } while (0)

#define DEC_VAR(var) \
    do               \
    {                \
        if (var > 0) \
        {            \
            var--;   \
        }            \
    } while (0)

#define NB_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))
#endif /* _MACROS_H_ */
