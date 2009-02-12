/*  Copyright 2008 Stephen English, Jeffrey Gough, Alexis Johnson, 
        Robert Spanton and Joanna A. Sun.

    This file is part of the Formica robot firmware.

    Foobar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Formica robot firmware.  
    If not, see <http://www.gnu.org/licenses/>.  */
#include "braitenberg.h"
#include "stdint.h"
#include "../motor.h"
#include "../adc10.h"
#include "../bearing.h"
#include "../leds.h"
#include "../types.h"
#include "../time.h"

#define HYSTERESIS 2
#define BB_PERIOD 1

void braitenberg_update( void )
{
  /* Updating braitenberg state machine too rapidly means robot 
   barely moves between updates. Noise in the bearing measurements 
   makes robot spasm on the spot, not really moving anywhere. So 
   limit update frequeny. */

  static uint32_t bbtime = 0;
  
  if (the_time > bbtime)
    {
      bbtime = the_time + BB_PERIOD;
	switch(bearing)
	{
	case 0:
		motor_mode = MOTOR_FWD;

		leds_set(ORANGE);

		if(pd_left>pd_right)
		{
			motor_l = 7;
			motor_r = 4;
		}
		else if (pd_right > pd_left)
		{
			motor_r = 7;
			motor_l = 4;
		}
		else
		  motor_r = motor_l = 7;
		break;

	case 120:
	        motor_mode = MOTOR_TURN_RIGHT;
		break;

	case 240:
		motor_mode = MOTOR_TURN_LEFT;
		break;
		
	default:
	  break;
	}
     }
 }

 void rev_braitenberg_update( void )
 {
  static uint32_t bbtime = 0;
  
  if (the_time > bbtime)
    {
      bbtime = the_time + BB_PERIOD;
	switch(bearing)
	{
		case 0:
			/* Turn right/left */
			motor_mode = MOTOR_TURN_LEFT;
			break;
		case 120:
		case 240:
			motor_mode = MOTOR_FWD;
			if(pd_left>(pd_right+HYSTERESIS))
			{
				/* Right stronger */
				motor_l = 2;
				motor_r = 6;
			}
			else if (pd_right > (pd_left+HYSTERESIS))
			{
				/* Left stronger */
				motor_r = 2;
				motor_l = 6;
			}
			else
				motor_r = motor_l = 7;

			break;
	}
    }
}

