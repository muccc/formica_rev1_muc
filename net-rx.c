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
#include "net-rx.h"
#include "net.h"
#include "device.h"
#include "leds.h"
#include "virus.h"
#include "random.h"
#include "motor.h"
#include "flash.h"
#include "net-tx.h"
#include "food.h"

uint16_t net_id = 0;

void net_rx_proc_incoming( uint8_t* frame, uint8_t len )
{
	/* Frames of length 0 aren't helpful */
	if( len == 0 )
		return;

	switch( frame[0] )
	{
	case NET_CMD_COLOUR:
		/* Receive a new colour */
		if( len != 3 )
			return;

		virus_set( frame[1], frame[2] );
		break;

	case NET_CMD_FW_BLOB:
	{
		/* Receive a blob of firmware */
		uint16_t fw_ver, chunk;
		if( len != 21 )
			return;

		fw_ver = (((uint16_t)frame[1]) << 8) | frame[2];
		chunk = (((uint16_t)frame[3]) << 8) | frame[4];

/* 		if( fw_ver > FIRMWARE_VERSION ) */
			flash_rx_chunk( chunk,
					(const uint16_t*)(frame + 7) );
		break;
	}

	case NET_CMD_FW_NEXT:
	{
		/* Receive a hint about what chunk to transmit next */
		uint16_t chunk;
		if( len != 3 )
			return;

		chunk = (((uint16_t)frame[1]) << 8) | frame[2];
		net_tx_chunk_hint(chunk);
		break;
	}

	case NET_CMD_FOOD:
	{
		uint32_t since = 0;
		uint8_t i;

		for( i=0; i<4; i-- )
			since = (since << 8) | frame[4-i];

		food_gotinfo( &since );
		break;
	}

	
	}
}

void net_rx_init( void )
{
	net_id = random();
}
