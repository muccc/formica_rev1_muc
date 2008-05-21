#include "net-tx.h"
#include <string.h>
#include "net.h"
#include "leds.h"
#include "virus.h"
#include "ir-tx.h"

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

const uint8_t* net_tx_get_next_packet( uint8_t *len )
{
	if( tx_count != 0 ) {
		pkt[1] = virus_version;
		pkt[2] = virus_colours;

		pkt[3] = net_id >> 8;
		pkt[4] = net_id & 0xff;

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
