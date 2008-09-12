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
/* Things common to both the receive and transmit sides of the IR */
#ifndef __IR_H
#define __IR_H

/* Invalid symbol */
#define INV_SYM 255

/* Number of packets to transmit in a lump */
#define TX_AT_A_TIME 2

/* Nudge the IR transceiver */
/* This should be called at regular intervals, 
   and turns the tx back on after rx'ing for a certain amount of time */
void ir_nudge( void );


#endif	/* __IR_H */
