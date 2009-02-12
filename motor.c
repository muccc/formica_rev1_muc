/*  Copyright 2008 Stephen English, Jeffrey Gough, Alexis Johnson, 
        Robert Spanton and Joanna A. Sun.

    This file is part of the Formica robot firmware.

    Foobar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Formica robot firmware.  
    If not, see <http://www.gnu.org/licenses/>.  */
#include "motor.h"
#include "device.h"
#include <signal.h>
#include "random.h"
#include "battery.h"
#include "ir.h"
#include "leds.h"
#include "bearing.h"
#include "time.h"
#include "behav/braitenberg.h"
#include "food.h"
#include "behav/parking.h"
#include "ir-tx.h"



motor_mode_t motor_mode = MOTOR_FWD;

uint8_t motor_r = 0;
uint8_t motor_l = 0;
void motor_rand_walk_change( void );
static uint8_t rand_walk_thresh = 0;
static bool random_walk_en = 0;

uint8_t MAX_SPEED = 8;

void random_walk_enable( void )
{
	random_walk_en = TRUE;
}

void random_walk_disable( void )
{
	random_walk_en = FALSE;
}

void motor_init( void )
{
	/* Set all as inputs -- all motors off */
	P1DIR &= ~M_ALL;

	/* Outputs need to be low when we enable the motors */
	P1OUT &= ~M_ALL;

	/* Configure the watchdog timer into interval mode */
	WDTCTL = WDTPW  	/* All writes to WDTCTL must have this password */
		/* WDTHOLD = 0 -- Watchdog time is enabled */
		/* WDTNMI = 0 -- RST pin is reset pin */
		| WDTTMSEL	/* Interval timer mode */
		| WDTCNTCL	/* Clear the timer */
		/* WDTSSEL = 0 -- Source clock from SMCLK */
		| WDTIS0;	/* Divide clock by 8192 */

	/* Enable the watchdog interrupt */
	IE1 |= WDTIE;
}

interrupt (WDT_VECTOR) motor_wdt_isr(void)
{
	static uint8_t count = 0;

	/* The resulting motor configuration (to be ORed with P1DIR) */
	uint8_t conf = 0;
	static uint16_t cc = 0;

	if( cc == 100 )
	{
		the_time++;
		food_level++;
		cc = 0;

		if(random_walk_en)
		{

			static uint8_t j = 0;

			if( j == rand_walk_thresh )
			{
				j = 0;
				motor_rand_walk_change();
			}

			j++;
		}
	}
	cc++;

	//	motor_mode = MOTOR_FWD;
	//motor_l = motor_r = 6;

	if( motor_mode == MOTOR_FWD )
	{
		MAX_SPEED = 8;

		conf = M_FWD;

		if( count >= motor_r )
			conf |= M2;

		if( count >= motor_l )
			conf |= M1;
	}

	if( motor_mode == MOTOR_BK )
	{
		MAX_SPEED = 8;

		conf = M_BK;

		if( count >= motor_r )
			conf &= ~M2;

		if( count >= motor_l )
			conf &= ~M1;
	}

	if( motor_mode == MOTOR_TURN_LEFT )
	{
		MAX_SPEED = 8;
		motor_l = 0;
		motor_r = 7;
		
		conf = M_FWD;

		if( count >= motor_r )
			conf |= M2;

		if( count >= motor_l )
			conf |= M1;
	}

	if( motor_mode == MOTOR_TURN_RIGHT )
	{
		MAX_SPEED = 8;
		motor_l = 7;
		motor_r = 0;
		
		conf = M_FWD;

		if( count >= motor_r )
			conf |= M2;

		if( count >= motor_l )
			conf |= M1;

	}

	motor_off();
	P1DIR |= conf;

	count++;
	if( count == MAX_SPEED )
		count = 0;

	/* don't transmit data when parking */
	if (!now_parking)
	  	  ir_nudge();
	else
	  ir_transmit_disable();
}

void motor_rand_walk_change( void )
{
	static uint8_t mode = 0;

	motor_r = motor_l = RAND_WALK_SPEED;
	switch(mode)
	{
	case 0:
		motor_mode = MOTOR_FWD;
		break;

	case 1:
		motor_mode = MOTOR_TURN_LEFT;
		break;

	case 2:
		if( !hasfood() )
			motor_mode = MOTOR_BK;
		else
			motor_mode = MOTOR_FWD;
		break;

	case 3:
		motor_mode = MOTOR_TURN_RIGHT;
		break;
	}

	mode = (random() >> 7) % 4;
	if( mode > 3 )
		mode = 0;

	rand_walk_thresh = (random() >> 5) + 1;
}

