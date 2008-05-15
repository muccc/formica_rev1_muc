#ifndef __BATTERY_H
#define __BATTERY_H
#include "types.h"

void battery_init( void );

bool battery_charge_complete( void );
bool battery_charge_in_progress( void );
bool battery_power_good( void );

#endif	/* __BATTERY_H */
