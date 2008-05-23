#include "parking.h"
#include "braitenberg.h"
#include "../motor.h"
#include "../battery.h"
#include "../bearing.h"
#include "../time.h"
#include "../types.h"
#include "../leds.h"

#define CHARGE_TIME (20 * 30)
#define FALLOUT_WAIT 10
#define OVERPUSH 8

bool charge_complete = FALSE;

void parking_update( void )
{
	static enum {
		NOTHIT,     /* Moving around in a random walk */
		JUSTHIT,    /* Just touched power */
		WEDGED,	    /* Been touching power for a while, wedged in */
		FALLEN      /* Was touching power, not at the moment */
	} hit = NOTHIT;

	static uint32_t t = 0; /* Time stop running motors after hitting the wall */
	static uint32_t e = 0; /* Time to give up pushing forward into the wall */
	static uint32_t c = 0; /* Time to finish charging */


	/* Charging algorithm:
	 * Random walk until power detected (state == NOTHIT)
	 * When power detected, store a time + 200ms goto state == JUSTHIT
	 * After 200ms if still got power goto state == WEDGED and turn off
	 * 	motors
	 */
	charge_complete = FALSE;
	leds_green_off();

	if(battery_power_good())
	{
		random_walk_disable();
		switch(hit)
		{
			case NOTHIT:
			case FALLEN:
				
				motor_l = motor_r = 4;
				
				t = the_time + OVERPUSH;
				hit = JUSTHIT;
				break;
			case JUSTHIT:
				motor_l = motor_r = 4;

				if(c == 0)
					c = the_time + CHARGE_TIME;
				
				if(t < the_time)
				{
					hit = WEDGED;
				}
				break;
			case WEDGED:
				leds_green_on();
				if(c < the_time)
				{
					/* Finished charging */
					charge_complete = TRUE;
					c = 0;
				}

				motor_l = motor_r = 0;

				break;
		}
	}
	else
	{
		switch(hit)
		{
			case NOTHIT:
				rev_braitenberg_update();
				break;
			case JUSTHIT:
			case WEDGED:
				motor_l = motor_r = 4;
				e = the_time + FALLOUT_WAIT;
				hit = FALLEN;
				break;
			case FALLEN:
				motor_l = motor_r = 4;
				if(e < the_time)
					hit = NOTHIT;
				break;
		}
	}
}
