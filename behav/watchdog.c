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
#include "watchdog.h"
#include "../bearing.h"
#include "../time.h"
#include "../motor.h"
#include "../leds.h"
#include "parking.h"

/* The time the watchdog will get hit */
static uint32_t thresh_time = 0;
/* The last bearing that the watchdog measured. */
static uint16_t watchdog_last_bearing = 0;

/* Buffer to contain previous bearing values */
#define B_HIST_LEN 8
static uint16_t b_hist[B_HIST_LEN];
static uint8_t bh_pos = 0;

void watchdog_update( void )
{
	if( the_time > thresh_time )
	{
		leds_flash(GREEN);
		motor_mode = MOTOR_BK;
		motor_l = motor_r = 5;

		time_wait(30);
 
		/* 2x threshold time if parking */
		if (now_parking)
			thresh_time = the_time + (WATCHDOG_THRESH << 1);
		else
			thresh_time = the_time + WATCHDOG_THRESH;
	}
}

void watchdog_bearing_change()
{
	if( bearing_strength >= 10 )
	{
		b_hist[bh_pos] = bearing;
		bh_pos++;
		if( bh_pos == B_HIST_LEN )
		{
			uint8_t i;
			uint16_t s = 0;

			bh_pos = 0;

			for(i=0; i<B_HIST_LEN; i++)
				s += b_hist[i];

			s /= B_HIST_LEN;
			if( s < 60 )
				s = 0;
			else if( s < 180 )
				s = 120;
			else if( s < 300 )
				s = 240;
			else 
				s = 0;

			if( s != watchdog_last_bearing )
			{
				/* 2x threshold time if parking */
				if (now_parking)
					thresh_time = the_time + (WATCHDOG_THRESH << 1);
				/*
				else if ( hasfood() && bearing == 0 )
				// When we have food, and we're headed towards the light,
				//   set the threshold to be much larger.
				thresh_time = the_time + (WATCHDOG_THRESH * 4);
				*/
				else
					thresh_time = the_time + WATCHDOG_THRESH;
			}
			watchdog_last_bearing = s;
		}
	}
}
