/* Makes the robot move if  */
#ifndef __WATCHDOG_H
#define __WATCHDOG_H
#include <stdint.h>

/* Check that the robot has moved. */
void watchdog_update( void );

void watchdog_bearing_change();

#endif	/* __WATCHDOG_H */
