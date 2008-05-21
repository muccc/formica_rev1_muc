#include "net-tx.h"
#include <string.h>
#include "net.h"
#include "leds.h"
#include "virus.h"
#include "ir-tx.h"
#include "flash.h"
#include "ir.h"
#include "random.h"

static uint8_t pkt[] = {
	/* Command */
	NET_CMD_COLOUR,
	/* Colour Version */
	0,
	/* Colour */
	0,
	/* Firmware version number (MSB, LSB) */
	0,0,
	/* Flash chunk number (MSB, LSB) */
	0,0,
	/* Chunk of flash */
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};

/* How many more packets to transmit */
static uint8_t tx_count = 0;

/* Which firmware chunk we're transmitting next */
static uint16_t tx_chunk = 0;

const uint8_t* net_tx_get_next_packet( uint8_t *len )
{
	if( tx_count != 0 ) {
		uint8_t p;
		uint8_t *chunk;
		uint16_t cnum;

		pkt[1] = virus_version;
		pkt[2] = virus_colours;

		pkt[3] = FIRMWARE_VERSION >> 8;
		pkt[4] = FIRMWARE_VERSION & 0xff;

		if( tx_count == TX_AT_A_TIME )
			cnum = random() % (LAST_CHUNK+1);
		else
			cnum = (tx_chunk + tx_count - 1) % (LAST_CHUNK+1);

		pkt[5] = cnum >> 8;
		pkt[6] = cnum & 0xff;

		chunk = (uint8_t*)flash_chunk_n( cnum );
		for( p=0; p < CHUNK_SIZE; p++ )
			pkt[p+7] = chunk[p];

		if( tx_count == 1 )
			tx_chunk ++;
		if( tx_chunk == LAST_CHUNK )
			tx_chunk = 0;

		*len = 23;
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
