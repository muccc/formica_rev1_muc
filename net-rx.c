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
