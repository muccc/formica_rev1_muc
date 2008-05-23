#ifndef __FOOD_H
#define __FOOD_H
#include "device.h"
#include "types.h"

/* Called with ADC data */
void foodcallback(uint16_t dataoff, uint16_t dataon);

bool hasfood(void);

void food_init(void);

#define NOFOOD 1000
#define FLED (1<<3)

/* Turn the food leds on/off */
#define fled_on() do { P4OUT |= FLED; } while (0)
#define fled_off() do { P4OUT &= ~FLED; } while (0)

extern uint32_t food_level;

/* Add another food time from someone else */
void food_gotinfo( uint32_t *since );

#endif	/* __FOOD_H */
