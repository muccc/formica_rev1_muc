#include "device.h"
#include "motor.h"
static uint16_t curbearing = 0;
static uint16_t strength = 0;

static uint16_t l = 0, r = 0;

void bearing_set(uint16_t a, uint16_t b, uint16_t c)
{
	l = c; r = b;
	if(a < b)
		if(a < c)
		{
			curbearing = 0;
			strength = a;
		}
		else
		{
			curbearing = 240;
			strength = c;
		}
	else
		if(b < c)
		{
			curbearing = 120;
			strength = b;
		}
		else
		{
			curbearing = 240;
			strength = c;
		}
}

void setmotorspeeds( void )
{
	motor_set_ratio(l, r);
}

uint16_t bearing_get( void )
{
	return curbearing;	
}

uint16_t bearing_get_strength( void )
{
	return strength;
}
