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
#include "smbus_pec.h"

#define POLY    (0x1070U << 3)

uint8_t crc8( uint8_t tempdata )
{
	uint8_t i;
	uint16_t data;

	data = (uint16_t)tempdata<<8;
	for(i = 0; i < 8; i++) 
	{
		if (data & 0x8000)
			data = data ^ POLY;
		data <<= 1;
	}

	return (data >> 8) & 0xFF;
}
