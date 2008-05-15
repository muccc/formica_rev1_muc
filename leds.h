#ifndef __LEDS_H
#define __LEDS_H
#include "device.h"

#define leds_init() do { P4OUT &= ~6; P4DIR |= 6; } while (0)

#define RED (2)
#define GREEN (4)

#define leds_red_on() do { P4OUT |= RED; } while (0)
#define leds_red_off() do { P4OUT &= ~RED; } while (0)

#define leds_green_on() do { P4OUT |= GREEN; } while (0)
#define leds_green_off() do { P4OUT &= ~GREEN; } while (0)

#endif	/* __LEDS_H */
