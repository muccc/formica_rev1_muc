#include "parking.h"
#include "../motor.h"
#include "../battery.h"
#include "../bearing.h"
#include "../time.h"
#include "../types.h"
#include "../leds.h"

#define CHARGE_TIME (20 * 20)
#define FALLOUT_WAIT 10
#define OVERPUSH 8

uint32_t time_charging = 0;
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
	leds_green_off();
	leds_red_off();
	charge_complete = FALSE;
	if(battery_power_good())
	{
		random_walk_disable();
		switch(hit)
		{
			case NOTHIT:
			case FALLEN:
				leds_red_on();

				motor_l = motor_r = 4;
				
				t = the_time + OVERPUSH;
				hit = JUSTHIT;
				break;
			case JUSTHIT:
				leds_red_on();

				motor_l = motor_r = 4;
				
				if(t < the_time)
				{
					hit = WEDGED;
					c = the_time + CHARGE_TIME;
				}
				break;
			case WEDGED:
				if(c < the_time)
					/* Finished charging */
					charge_complete = TRUE;

				motor_l = motor_r = 0;
				leds_green_on();
				leds_red_on();

				break;
		}
	}
	else
	{
		switch(hit)
		{
			case NOTHIT:
				random_walk_enable();
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
