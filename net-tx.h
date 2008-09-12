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
/* Deals with the network layer of things */
#ifndef __NET_TX_H
#define __NET_TX_H
#include <stdint.h>

void net_tx_init( void );

/* Returns the next packet to be sent */
const uint8_t* net_tx_get_next_packet( uint8_t *len );

/* Enable transmission for n packets */
void net_tx_enable_for( uint8_t n );

/* Hint which chunk of firmware to transmit next */
void net_tx_chunk_hint( uint16_t chunk );

#endif	/* __NET_TX_H */
