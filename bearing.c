#include "device.h"
#include "motor.h"

uint16_t bearing = 0;
uint16_t bearing_strength = 0;

void bearing_set( uint16_t *pdr )
{
	if(pdr[0] < pdr[1])
		if(pdr[0] < pdr[2])
		{
			bearing = 0;
			bearing_strength = pdr[0];
		}
		else
		{
			bearing = 240;
			bearing_strength = pdr[2];
		}
	else
		if(pdr[1] < pdr[2])
		{
			bearing = 120;
			bearing_strength = pdr[1];
		}
		else
		{
			bearing = 240;
			bearing_strength = pdr[2];
		}
}
