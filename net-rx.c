#include "net-rx.h"
#include "net.h"
#include "device.h"
#include "leds.h"
#include "virus.h"
#include "random.h"
#include "motor.h"

uint16_t net_id = 0;

void net_rx_proc_incoming( uint8_t* frame, uint8_t len )
{
	uint8_t l = len;

	/* Frames of length 0 aren't helpful */
	if( len == 0 )
		return;

	switch( frame[0] )
	{
	case NET_CMD_COLOUR:
		if( len != 23 )
			return;

		//virus_set( frame[1], frame[2] );

		
		break;
	}
}

void net_rx_init( void )
{
	net_id = random();
}
