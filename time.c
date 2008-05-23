#include "time.h"

uint32_t the_time;

void time_wait( uint16_t t )
{
	uint32_t b = the_time + t;

	while( b > the_time );
}
