#include "virus.h"
#include "leds.h"

uint8_t virus_colours = GREEN;
uint8_t virus_version = 2;

static void virus_conf( void );

void virus_init( void )
{
	virus_conf();
}

void virus_set( uint8_t version, uint8_t colours )
{
	if( virus_version < version )
	{
		virus_version = version;
		virus_colours = colours;
		virus_conf();
	}
}

static void virus_conf( void )
{
	if( virus_colours & RED )
		leds_red_on();
	else
		leds_red_off();

	if( virus_colours & GREEN )
		leds_green_on();
	else
		leds_green_off();
}
