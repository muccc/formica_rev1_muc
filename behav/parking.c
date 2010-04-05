/*  Copyright 2008 Stephen English, Jeffrey Gough, Alexis Johnson, 
    Robert Spanton and Joanna A. Sun.

    This file is part of the Formica robot firmware.

    The Formica robot firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Formica robot firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Formica robot firmware.  
    If not, see <http://www.gnu.org/licenses/>.  */
#include "parking.h"
#include "braitenberg.h"
#include "watchdog.h"
#include "../food.h"
#include "../motor.h"
#include "../battery.h"
#include "../bearing.h"
#include "../time.h"
#include "../types.h"
#include "../leds.h"

#define CHARGE_TIME (20L * 60 * 60)	/* Each tick is 50ms, so 20 ticks per second */
#define FALLOUT_WAIT 60
#define RUNUP_WAIT 20
#define OVERPUSH 2

#define ANGER_MILD_PUSH_TIME (1 * TICKS_PER_SEC)
#define ANGER_NUDGE_TIME (5 * TICKS_PER_SEC)

#define NUDGE_TIME (TICKS_PER_SEC / 6)

bool charge_complete = FALSE;
bool now_parking = FALSE;

void parking_update( void )
{
	static enum {
		PS_PARK,
		PS_CHARGING,	/* We're charging */
		PS_KNOCKED_OFF_ANGER1,
		PS_KNOCKED_OFF_ANGER2,
	} pstate = PS_PARK;
	static uint32_t t = 0;

	/* Immediately jump to the charging state... */
	if( battery_power_good() )
		pstate = PS_CHARGING;

	switch( pstate )
	{
	case PS_PARK:
		/*** We're heading to the charger ***/

		/* Avoid food */
		if( hasfood() )
		{
			motor_l = motor_r=6;
			motor_mode = MOTOR_BK;
			time_wait(6);
			motor_mode = MOTOR_TURN_LEFT;
			time_wait(3);
			motor_mode = MOTOR_FWD;
			time_wait(3);
			motor_mode = MOTOR_TURN_RIGHT;
			time_wait(3);
			motor_l = motor_r = 0;
		}

		rev_braitenberg_update();
		/* Get unstuck if we can't move on our way to the charger */
		watchdog_update();
		break;

	case PS_CHARGING:
		/*** We've reached the charger ***/

		random_walk_disable();
		/* Turn the motors off */
		motor_l = motor_r = 0;
		mood = MOOD_CHARGING;

		/* Finished charging? */
		if( battery_charge_complete() )
		{
			mood = MOOD_NONE;
			tempmood = MOOD_CHARGED;
			charge_complete = TRUE;
		}
		/* Lost contact with charger? */
		else if( !battery_power_good() ) {
			pstate = PS_KNOCKED_OFF_ANGER1;
			t = the_time + ANGER_MILD_PUSH_TIME;

			motor_mode = MOTOR_FWD;
			motor_l = motor_r = 2;
		}
		break;

	case PS_KNOCKED_OFF_ANGER1:
		/*** We were knocked off the charger ***/
		mood = MOOD_CHARGING_ANGER1;

		if( the_time > t ) {
			pstate = PS_KNOCKED_OFF_ANGER2;
			t = the_time + ANGER_NUDGE_TIME;
		}
		break;

	case PS_KNOCKED_OFF_ANGER2:
		/*** We were severely knocked off the charger ***/
		mood = MOOD_CHARGING_ANGER2;

		/* Reverse a little, then drive forwards again */
		motor_l = motor_r = 6;

		motor_mode = MOTOR_BK;
		time_wait(NUDGE_TIME);

		motor_mode = MOTOR_FWD;
		/* Drive forwards for more time, to make sure we get to the rail */
		time_wait(NUDGE_TIME * 2);

		/* Leave the motors driving forwards mildly */
		motor_l = motor_r = 3;

		if( the_time > t )
			/* Revert to reverse braitenberg again */
			pstate = PS_PARK;
		break;
	}
}
