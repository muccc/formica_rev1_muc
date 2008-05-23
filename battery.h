#ifndef __BATTERY_H
#define __BATTERY_H
#include "types.h"
#include "stdint.h"

void battery_init( void );

bool battery_charge_complete( void );
bool battery_charge_in_progress( void );
bool battery_power_good( void );

bool battery_critical( void );
bool battery_low( void );

extern uint16_t battval;

#endif	/* __BATTERY_H */
