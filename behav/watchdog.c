#include "watchdog.h"
#include "../bearing.h"
#include "../time.h"
#include "../motor.h"
#include "../leds.h"

static uint32_t thresh_time = 0;
static uint16_t watchdog_last_bearing = 0;

#define B_HIST_LEN 8
static uint16_t b_hist[B_HIST_LEN];
static uint8_t bh_pos = 0;

void watchdog_update( void )
{
	if( the_time > thresh_time )
	{
		motor_mode = MOTOR_BK;
		motor_l = motor_r = 5;

		time_wait(40);

		thresh_time = the_time + (20 * 4);
	}
}

void watchdog_bearing_change()
{
	if( bearing_strength < 10 )
		return;

	b_hist[bh_pos] = bearing;
	bh_pos++;
	if( bh_pos == B_HIST_LEN )
	{
		uint8_t i;
		uint16_t s = 0;

		bh_pos = 0;

		for(i=0; i<B_HIST_LEN; i++)
			s += b_hist[i];

		s /= B_HIST_LEN;
		if( s < 60 )
			s = 0;
		else if( s < 180 )
			s = 120;
		else if( s < 300 )
			s = 240;
		else 
			s = 0;

		if( s != watchdog_last_bearing )
			thresh_time = the_time + (20 * 4);

		watchdog_last_bearing = s;
	}
}