#include "device.h"
#include "motor.h"

uint16_t bearing = 0;
uint16_t bearing_strength = 0;
uint16_t light_intensity = 0

void bearing_set( uint16_t *pdr )
{
	if(pdr[0] < pdr[1])
		if(pdr[0] < pdr[2])
		{
			bearing = 0;
			light_intensity = pdf[0];
			if pdr[1] > pdr[2]
				bearing_strength = pdr[1] - pdr[0];
			else
				bearing_strength = pdr[2] - pdr[0];
		}
		else
		{
			bearing = 240;
			light_intensity = pdf[2];
			if pdr[1] > pdr[0]
				bearing_strength = pdr[1] - pdr[2];
			else
				bearing_strength = pdr[0] - pdr[2];
		}
	else
		if(pdr[1] < pdr[2])
		{
			bearing = 120;
			light_intensity = pdf[1];
			if pdr[2] > pdr[0]
				bearing_strength = pdr[2] - pdr[1];
			else
				bearing_strength = pdr[0] - pdr[1];
		}
		else
		{
			bearing = 240;
			light_intensity = pdf[2];
			if pdr[1] > pdr[0]
				bearing_strength = pdr[1] - pdr[2];
			else
				bearing_strength = pdr[0] - pdr[2];
		}
}
