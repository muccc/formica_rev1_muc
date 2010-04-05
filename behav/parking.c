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

bool charge_complete = FALSE;
bool now_parking = FALSE;

void parking_update( void )
{
	static enum {
		NOTHIT,     /* Moving around in a random walk */
		JUSTHIT,    /* Just touched power */
		WEDGED,	    /* Been touching power for a while, wedged in */
		FALLEN,     /* Was touching power, not at the moment */
		ANOTHERRUNUP 	/* Can't reastablish contact. Having a second run-up */
	} pstate = NOTHIT;

	static uint32_t t = 0; /* Time stop running motors after hitting the wall */
	static uint32_t r = 0; /* Time to go for a run-up */
	static uint32_t c = 0; /* Time to finish charging */
	static uint32_t e = 0; /* Time to give up pushing forward into the wall */

	/* Charging algorithm:
	 * Random walk until power detected (state == NOTHIT)
	 * When power detected, store a time + 200ms goto state == JUSTHIT
	 * After 200ms if still got power goto state == WEDGED and turn off
	 * 	motors
	 */

	if(battery_power_good())
	{
		random_walk_disable();
		switch(pstate)
		{
		case NOTHIT:
		case FALLEN:
		case ANOTHERRUNUP:
			motor_mode = MOTOR_FWD;
			motor_l = motor_r = 6;
				
			t = the_time + OVERPUSH;
			pstate = JUSTHIT;
			break;
		case JUSTHIT:
			motor_mode = MOTOR_FWD;
			motor_l = motor_r = 6;

			if(c == 0)
				c = the_time + CHARGE_TIME;
				
			if(t < the_time)
			{
				pstate = WEDGED;
			}
			break;
		case WEDGED:
			mood = MOOD_CHARGING;
			if(c < the_time)
			{
				/* Bored of charging */
				mood = MOOD_NONE;
				tempmood = MOOD_BORED_CHARGING;
				charge_complete = TRUE;
				c = 0;
				pstate = NOTHIT;
			}
			if (battery_charge_complete())
			{
				/* finished charging */
				mood = MOOD_NONE;
				tempmood = MOOD_CHARGED;
				charge_complete = TRUE;
				c = 0;
				pstate = NOTHIT;
			}

			motor_l = motor_r = 0;

			break;
		}
	}
	else			/* not touching the charger */
	{
		switch(pstate)
		{
		case NOTHIT:
			c = 0;
			rev_braitenberg_update();
			watchdog_update();
			/* want the stuck-on-object watchdog active */
			/* whilst seeking charger */
			if (hasfood())
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
				motor_mode = MOTOR_FWD;
			}
			break;
		case JUSTHIT:
		case WEDGED:
			mood = MOOD_DRIVING_TO_CHARGER_NOFOOD;
			motor_mode = MOTOR_FWD;
			motor_l = motor_r = 6;
			r = the_time + RUNUP_WAIT;
			pstate = FALLEN;
			break;
		case FALLEN:
			motor_l = motor_r = 6;
			motor_mode = MOTOR_FWD;
				
			e = the_time + FALLOUT_WAIT;

			if(r < the_time)
				pstate = ANOTHERRUNUP;
			if(e < the_time)
				pstate = NOTHIT;
			
			break;
		case ANOTHERRUNUP:
			motor_l = motor_r = 6;
			motor_mode = MOTOR_BK;
			time_wait(2);
			motor_mode = MOTOR_FWD;	
			
			if(e < the_time)
				pstate = NOTHIT;
			break;
		}
	}
}
