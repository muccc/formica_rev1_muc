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
#include "virus.h"
#include "leds.h"

uint8_t virus_colours = RED;
uint8_t virus_version = 1;

static void virus_conf( void );

void virus_init( void )
{
	virus_conf();
}

void virus_set( uint8_t version, uint8_t colours )
{
	if( virus_version < version )
	{
		virus_version = version;
		virus_colours = colours;
		virus_conf();
	}
}

static void virus_conf( void )
{
	if( virus_colours & RED )
		leds_red_on();
	else
		leds_red_off();

	if( virus_colours & GREEN )
		leds_green_on();
	else
		leds_green_off();
}
