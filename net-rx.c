#include "net-rx.h"
#include "net.h"
#include "device.h"
#include "leds.h"
#include "virus.h"

void net_rx_proc_incoming( uint8_t* frame, uint8_t len )
{
	/* Frames of length 0 aren't helpful */
	if( len == 0 )
		return;

	switch( frame[0] )
	{
	case NET_CMD_COLOUR:
		if( len != 3 )
			return;

		virus_set( frame[1], frame[2] );
		break;
	}
}
