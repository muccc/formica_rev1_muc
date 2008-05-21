#include "device.h"
#include "motor.h"

uint16_t bearing = 0;
uint16_t bearing_strength = 0;

static uint16_t l = 0, r = 0;

void bearing_set(uint16_t a, uint16_t b, uint16_t c)
{
	l = c; r = b;
	if(a < b)
		if(a < c)
		{
			bearing = 0;
			bearing_strength = a;
		}
		else
		{
			bearing = 240;
			bearing_strength = c;
		}
	else
		if(b < c)
		{
			bearing = 120;
			bearing_strength = b;
		}
		else
		{
			bearing = 240;
			bearing_strength = c;
		}
}

void setmotorspeeds( void )
{
	motor_set_ratio(l, r);
}
