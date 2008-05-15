#include "virus.h"
#include "leds.h"

uint8_t cur_leds = RED;
uint8_t cur_led_version = 2;

void virus_init( void )
{
	
}

void virus_set( uint8_t version, uint8_t colours )
{
	if( cur_led_version < version )
	{
		if( colours & RED )
			leds_red_on();
		else
			leds_red_off();

		if( colours & GREEN )
			leds_green_on();
		else
			leds_green_off();

		cur_led_version = version;
		cur_leds = colours;
	}
}
