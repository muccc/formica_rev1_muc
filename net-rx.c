#include "net-rx.h"
#include "net.h"
#include "device.h"
#include "leds.h"

uint8_t cur_leds = 0;
uint8_t cur_led_version = 0;

void net_rx_proc_incoming( uint8_t* frame, uint8_t len )
{
	/* Frames of length 0 aren't helpful */
	if( len == 0 )
		return;

	switch( frame[0] )
	{
	case NET_CMD_COLOUR:
	{
		if( len != 3 )
			return;

		if( cur_led_version < frame[1] )
		{
			cur_led_version = frame[1];
			cur_leds = frame[2];
			
			if( frame[2] & RED )
				leds_red_on();
			else
				leds_red_off();

			if( frame[2] & GREEN )
				leds_green_on();
			else
				leds_green_off();
		}
		
		break;
	}
	}
}
