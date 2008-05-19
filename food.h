#ifndef __FOOD_H
#define __FOOD_H
#include "device.h"

void foodcallback(uint16_t data);
uint8_t hasfood(void);
void food_init(void);

#define NOFOOD 1000
#define FLED (1<<3)

#define fled_init() do { P4OUT &= ~FLED; P4DIR |= FLED; } while (0)

#define fled_on() do { P4OUT |= FLED; } while (0)
#define fled_off() do { P4OUT &= ~FLED; } while (0)


#endif	/* __FOOD_H */
