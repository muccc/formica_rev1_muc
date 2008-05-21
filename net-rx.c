#include "net-rx.h"
#include "net.h"
#include "device.h"
#include "leds.h"
#include "virus.h"
#include "random.h"
#include "motor.h"
#include "flash.h"

uint16_t net_id = 0;

void net_rx_proc_incoming( uint8_t* frame, uint8_t len )
{
	/* Frames of length 0 aren't helpful */
	if( len == 0 )
		return;

	switch( frame[0] )
	{
	case NET_CMD_COLOUR: {
		uint16_t fw_ver;
		if( len != 23 )
			return;

		//virus_set( frame[1], frame[2] );

		fw_ver = (((uint16_t)frame[3]) << 8) | frame[4];

/* 		if( fw_ver > FIRMWARE_VERSION ) */
			flash_rx_chunk( (((uint16_t)frame[5]) << 8) | frame[6],
					(const uint16_t*)(frame + 7) );
		
		break;
	}
	}
}

void net_rx_init( void )
{
	net_id = random();
}
