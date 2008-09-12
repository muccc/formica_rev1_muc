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
#include "ir-tx-data.h"
#include "freq.h"
#include "device.h"
#include "types.h"
#include "net-tx.h"
#include <stddef.h>
#include "smbus_pec.h"
#include "ir.h"

/* Generates the next byte to transmit */
static bool next_byte( uint8_t* b );

#if CONF_TX_SEQ==1
uint8_t ir_tx_next_symbol( void )
{
	static uint8_t last_sym = 0;
	uint8_t sym;

	if( last_sym == NFREQ-1 )
		sym = 0;
	else
		sym = last_sym+1;

	last_sym = sym;
	return sym;

}
#else
uint8_t ir_tx_next_symbol( void )
{
	static uint8_t curbyte;
	/* How many symbols have been transmitted from the current byte */
	static uint8_t cb_pos = 0;
	static uint8_t last_sym = 255;

	uint8_t sym;

	if( cb_pos == 0 ) {
		/* Send the start byte symbol */
		sym = 0;
		if( !next_byte( &curbyte ) )
			return INV_SYM;
	} else {
		/* Grab the data from the byte */
		sym = curbyte & SYM_MASK;
		curbyte >>= NBITS;
			
		/* Now munge up the symbol so that we don't 
		   transmit the same one again! 
		   (You know... the rotating thing) */
		
		/* First, add 1 because symbol 0 is the 'start bit' symbol */
		sym++;

		/* Avoid the symbol that was transmitted last */
		if( sym >= last_sym )
			sym++;
	}
	cb_pos++;

	if( cb_pos == SYMBOLS_PER_BYTE+1 )
		cb_pos = 0;

	last_sym = sym;
	return sym;
}
#endif

static bool next_byte( uint8_t* b )
{
	/* The current packet that's being transmitted */
	static const uint8_t *cur_packet = NULL;
	/* The length of the current packet */
	static uint8_t cp_len = 0;

	if( cur_packet == NULL )
		cur_packet = net_tx_get_next_packet( &cp_len );

	if( cur_packet != NULL ) {
		static uint8_t p = 0;
		static uint8_t checksum = 0;
		static bool escaped = FALSE;

		if( p == 0 ) {
			/* Send the 'start of frame' byte */
			*b = 0x7E;
			checksum = 0;
		}
		else {
			if( p == 1 ) {
				*b = cp_len;
				if( !escaped )
					checksum_add(checksum, *b);
			}
			else if( p < (cp_len + 2) ) {
				*b = cur_packet[ p - 2 ];
				if( !escaped )
					checksum_add(checksum, *b);
			}
			else
				/* The checksum */
				*b = checksum;

			if( !escaped && (*b == 0x7e || *b == 0x7d) ) {
				escaped = TRUE;
				*b = 0x7d;
			} 
			else if( escaped ) {
				escaped = FALSE;
				*b ^= 0x20;
			}
		}

		if( !escaped ) {
			if( p == cp_len + 2 )
			{
				p = 0;
				cur_packet = net_tx_get_next_packet( &cp_len );
			}
			else
				p++;
		}

		return TRUE;
	}
	else
		return FALSE;
}
