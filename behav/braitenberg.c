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

void braitenberg_update( void )
{
	switch(bearing)
	{
	case 0:
		motor_mode = MOTOR_FWD;

		if(pd_left>pd_right)
		{
			motor_l = 6;
			motor_r = 2;
		}
		else if (pd_right > pd_left)
		{
			motor_r = 6;
			motor_l = 2;
		}
		else
			motor_r = motor_l = 5;
		break;

	case 120:
		motor_mode = MOTOR_TURN_RIGHT;
		motor_r = motor_l = 5;
		break;

	case 240:
		motor_mode = MOTOR_TURN_LEFT;
		motor_r = motor_l = 5;
		break;

	default:
		break;
	}
}

void rev_braitenberg_update( void )
{
	switch(bearing)
	{
		case 0:
			/* Turn right/left */
			motor_mode = MOTOR_TURN_RIGHT;
			motor_r = motor_l = 5;
			break;
		case 120:
		case 240:
			motor_mode = MOTOR_FWD;
			if(pd_left>(pd_right+HYSTERESIS))
			{
				/* Right stronger */
				motor_l = 1;
				motor_r = 5;
			}
			else if (pd_right > (pd_left+HYSTERESIS))
			{
				/* Left stronger */
				motor_r = 1;
				motor_l = 5;
			}
			else
				motor_r = motor_l = 5;

			break;
	}
}

