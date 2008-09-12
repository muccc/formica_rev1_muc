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
#ifndef __LEDS_H
#define __LEDS_H
#include "device.h"

#define leds_init() do { P4OUT &= ~6; P4DIR |= 6; } while (0)

#define RED (2)
#define GREEN (4)

#define leds_red_on() do { P4OUT |= RED; } while (0)
#define leds_red_off() do { P4OUT &= ~RED; } while (0)

#define leds_green_on() do { P4OUT |= GREEN; } while (0)
#define leds_green_off() do { P4OUT &= ~GREEN; } while (0)

#endif	/* __LEDS_H */
