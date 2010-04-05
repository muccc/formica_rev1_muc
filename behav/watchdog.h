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
/* Makes the robot move if  */
#ifndef __WATCHDOG_H
#define __WATCHDOG_H
#include <stdint.h>

#define WATCHDOG_THRESH 30

/* Check that the robot has moved. */
void watchdog_update( void );

/* Get the watchdog to take note of a new bearing reading. */
void watchdog_bearing_change();

#endif	/* __WATCHDOG_H */
