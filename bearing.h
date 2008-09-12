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
#ifndef __BEARING_H
#define __BEARING_H
#include <stdint.h>

/* Set the bearing with new ADC readings.
   Argument:
   - pdr: Array of photodiode readings (length 3). */
void bearing_set( uint16_t* pdr );

/* Get the bearing (0-359). */
#define bearing_get() ( bearing )

/* Get the maximum measured light intensity (0-1023) */
#define bearing_get_strength() ( bearing_strength )

extern uint16_t bearing;
extern uint16_t bearing_strength;
extern uint16_t light_intensity;

#endif
