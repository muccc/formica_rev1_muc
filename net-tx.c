#include "net-tx.h"
#include <string.h>
#include "net.h"
#include "leds.h"
#include "virus.h"
#include "ir-tx.h"
#include "flash.h"
#include "ir.h"
#include "random.h"
#include "time.h"
#include "food.h"

static uint8_t pkt[32];

/*	 
 0: NET_CMD_COLOUR
 1: Colour Version
 2: Colour */

/*
  0: NET_CMD_FW_BLOB
  1,2: Firmware version number (MSB, LSB)
  3,4: Flash chunk number (MSB, LSB)
  5-20:	16-byte Chunk of flash 
*/

/*
  0: NET_CMD_FW_NEXT
  1,2: Next chunk that we need (MSB, LSB)
*/

/* 
  0: NET_CMD_FOOD
  1,2,3,4: Time since last food (MSB...LSB)
 */


/* How many more packets to transmit */
static uint8_t tx_count = 0;

/* Which frame type we're transmitting next */
static uint8_t next_type = 0;

/* Which chunks we've received hints about */
#define NEXT_CHUNKS_LEN 3
static uint16_t next_chunks[NEXT_CHUNKS_LEN];
static uint8_t nc_pos = 0;
/* Next in the list to be transmitted */
static uint8_t nc_tx_pos = 0;

void net_tx_init( void )
{
	uint8_t i;
	for( i=0; i < NEXT_CHUNKS_LEN; i++ )
		next_chunks[i] = i;
}

const uint8_t* net_tx_get_next_packet( uint8_t *len )
{
	if( tx_count != 0 ) {
		pkt[0] = next_type;

		switch( next_type )
		{
		case NET_CMD_COLOUR:
			*len = 3;

			pkt[1] = virus_version;
			pkt[2] = virus_colours;
			break;

		case NET_CMD_FW_BLOB:
		{
			uint16_t cnum;
			uint8_t *chunk, p;
			*len = 21;

			pkt[1] = FIRMWARE_VERSION >> 8;
			pkt[2] = FIRMWARE_VERSION & 0xff;

			cnum = next_chunks[ nc_tx_pos ];

			/* Increment the chunk number in this slot */
			next_chunks[nc_tx_pos]++;
			if( next_chunks[nc_tx_pos] == (LAST_CHUNK+1) )
				next_chunks[nc_tx_pos] = 0;			

			/* Move onto the next slot */
			nc_tx_pos++;
			if( nc_tx_pos == NEXT_CHUNKS_LEN )
				nc_tx_pos = 0;

			pkt[3] = cnum >> 8;
			pkt[4] = cnum & 0xff;

			chunk = (uint8_t*)flash_chunk_n( cnum );
			for( p=0; p < CHUNK_SIZE; p++ )
				pkt[p+5] = chunk[p];

			break;
		}

		case NET_CMD_FW_NEXT:
			*len = 3;
			pkt[1] = next_chunk >> 8;
			pkt[2] = next_chunk & 0xff;
			break;

		case NET_CMD_FOOD:
		{
			*len = 5;

			pkt[1] = food_level >> 24;
			pkt[2] = (food_level >> 16) & 0xff;
			pkt[3] = (food_level >> 8) & 0xff;
			pkt[4] = food_level & 0xff;;
			break;
		}

		}

		next_type++;
		if( next_type == NET_CMD_LAST )
			next_type = 0;

		tx_count--;
		return pkt;
	} else {
		*len = 0;
		return NULL;
	}
}

void net_tx_enable_for( uint8_t n )
{
	tx_count = n;

	if( n != 0 )
		ir_transmit_enable();
}

void net_tx_chunk_hint( uint16_t chunk )
{
	if( chunk <= LAST_CHUNK ) {
		uint8_t i;

		/* Check the chunk isn't already in the queue */
		for( i=0; i<NEXT_CHUNKS_LEN; i++ )
			if( next_chunks[i] == chunk )
				return;

		next_chunks[nc_pos] = chunk;
		nc_pos++;
		if( nc_pos == NEXT_CHUNKS_LEN )
			nc_pos = 0;
	}	
}
