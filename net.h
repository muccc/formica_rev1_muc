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
#ifndef __NET_H
#define __NET_H

enum
{
	/* Colour command */
	NET_CMD_COLOUR,

	/* Firmware blob */
	NET_CMD_FW_BLOB,

	/* Ask for the next firmware blob */
	NET_CMD_FW_NEXT,

	/* Last time I saw food */
	NET_CMD_FOOD,

	NET_CMD_LAST
};

extern uint16_t net_id;

#endif	/* __NET_H */
