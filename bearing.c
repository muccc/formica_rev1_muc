#include "device.h"
#include "motor.h"
uint16_t curbearing = 0;
uint16_t bearing_strength = 0;

static uint16_t l = 0, r = 0;

void bearing_set(uint16_t a, uint16_t b, uint16_t c)
{
	l = c; r = b;
	if(a < b)
		if(a < c)
		{
			curbearing = 0;
			bearing_strength = a;
		}
		else
		{
			curbearing = 240;
			bearing_strength = c;
		}
	else
		if(b < c)
		{
			curbearing = 120;
			bearing_strength = b;
		}
		else
		{
			curbearing = 240;
			bearing_strength = c;
		}
}

void setmotorspeeds( void )
{
	motor_set_ratio(l, r);
}
