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
#ifndef __SMBUS_PEC_H
#define __SMBUS_PEC_H
#include <stdint.h>

#define checksum_add(C,B) do { C = crc8( C ^ B ); } while (0)

uint8_t crc8( uint8_t tempdata );

#endif	/* __SMBUS_PEC_H */
