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
#include "device.h"
#include "motor.h"
#include "behav/watchdog.h"

uint16_t bearing = 0;
uint16_t bearing_strength = 0;
uint16_t light_intensity = 0;

void bearing_set( uint16_t *pdr )
{
	if(pdr[0] < pdr[1])
	{
		if(pdr[0] < pdr[2])
		{
			bearing = 0;
			light_intensity = pdr[0];
			if (pdr[1] > pdr[2])
				bearing_strength = pdr[1] - pdr[0];
			else
				bearing_strength = pdr[2] - pdr[0];
		}
		else
		{
			bearing = 240;
			light_intensity = pdr[2];
			if (pdr[1] > pdr[0])
				bearing_strength = pdr[1] - pdr[2];
			else
				bearing_strength = pdr[0] - pdr[2];
		}
	}
	else
	{
		if(pdr[1] < pdr[2])
		{
			bearing = 120;
			light_intensity = pdr[1];
			if (pdr[2] > pdr[0])
				bearing_strength = pdr[2] - pdr[1];
			else
				bearing_strength = pdr[0] - pdr[1];
		}
		else
		{
			bearing = 240;
			light_intensity = pdr[2];
			if (pdr[1] > pdr[0])
				bearing_strength = pdr[1] - pdr[2];
			else
				bearing_strength = pdr[0] - pdr[2];
		}
	}
	watchdog_bearing_change();
}
