#include "time.h"
#include "device.h"

volatile uint32_t the_time = 0;

void time_wait( uint32_t t )
{
	uint32_t b = the_time + t;

	while( b > the_time )
	{
	}
}
