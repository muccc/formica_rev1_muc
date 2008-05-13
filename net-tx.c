#include "net-tx.h"
#include <string.h>
#include "net.h"
#include "leds.h"

static uint8_t pkt[] = {
	/* Command */
	NET_CMD_COLOUR,
	/* Colour Version */
	0,
	/* Colour */
	0
};

const uint8_t* net_tx_get_next_packet( uint8_t *len )
{
	pkt[1] = cur_led_version;
	pkt[2] = cur_leds;

	*len = 3;
	return pkt;
}
