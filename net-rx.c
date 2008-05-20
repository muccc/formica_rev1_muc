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
	/* Frames of length 0 aren't helpful */
	if( len == 0 )
		return;

	switch( frame[0] )
	{
	case NET_CMD_COLOUR:
		if( len != 5 )
			return;

		//virus_set( frame[1], frame[2] );

/* 		if( ((((uint16_t)frame[3]) << 8) | frame[4])  == net_id ) */
/* 			motor_rand_walk_change(); */
		
		break;
	}
}

void net_rx_init( void )
{
	net_id = random();
}
