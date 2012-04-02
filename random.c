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
#include "random.h"
#include "adc10.h"
#include "device.h"
#include <signal.h>

uint16_t lfsr;

void random_init( void )
{
	/* Seed with 2342 */
	lfsr = 2342;

	/*Spin a few times to spice it up a bit*/
	random();
	random();
	random();
}

uint16_t random( void )
{
	/* Rotate the 10 bit LFSR */
	/* Bit 9 xor Bit 6 xor 1 */
	lfsr = (lfsr >> 1) | ((uint16_t)( ((lfsr & 0x0200) >> 9) ^
					  ((lfsr & 0x0040) >> 6) ^
					 1) << 0x09);
	return lfsr;
}
