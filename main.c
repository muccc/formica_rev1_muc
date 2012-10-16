/*  Copyright 2008 Stephen English, Jeffrey Gough, Alexis Johnson, 
    Robert Spanton and Joanna A. Sun.

    This file is part of the Formica robot firmware.

    The Formica robot firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Formica robot firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Formica robot firmware.  
    If not, see <http://www.gnu.org/licenses/>.  */

/* Standard includes. */
#include <msp430f2254.h>
#include <isr_compat.h>
#include <stdint.h>
#include "interrupt.h"
#include "ir-bias.h"
#include "opamp-1.h"
#include "adc10.h"
#include "random.h"
#include "motor.h"
#include "leds.h"
#include "battery.h"
#include "bearing.h"
#include "time.h"
#include "behav/braitenberg.h"
#include "behav/parking.h"
#include "behav/watchdog.h"

/* when food_level rises above this amount, go to charger */
#define FOOD_THRESHOLD (20 * 60)

/* Initialises everything. */
void init(void);

/* Performs emergency power off.
   Never returns. */
void low_power(void);

int i = 0;

int main( void )
{
    //Initialize everything
    init();

    random_walk_disable();

    time_wait(TICKS_PER_SEC * 1);
    /* start charging if touching charger within 1 second */
    leds_set(RED);                // red LED on for 2 seconds
    P4DIR |= (0x09);                 //P40 and P43 are output IR
    P4OUT |= (0x09);            //all IR leds on (3 top, one bottom)
    time_wait(TICKS_PER_SEC * 2);
    leds_set(GREEN);          //green LED on for 1 second
    time_wait(TICKS_PER_SEC * 1);
    leds_set(ORANGE);          //green LED on for 1 second
    time_wait(TICKS_PER_SEC * 1);
    leds_set(RED);          //green LED on for 1 second
    time_wait(TICKS_PER_SEC * 1);
    leds_set(NONE);
    P4OUT &= ~(0x01);
    motor_l=1;              //left motor running at low speed for 1 second
    time_wait(TICKS_PER_SEC *1);
    motor_l=0;
    motor_r=1;              //right motor running at low speed for 1 second
    time_wait(TICKS_PER_SEC *1);
    motor_r=0;

    motor_mode= MOTOR_BK;
    time_wait(TICKS_PER_SEC *2);
    motor_l=2;
    motor_r=2;
    time_wait(TICKS_PER_SEC *1);
    motor_l=0;
    motor_r=0;
    motor_mode= MOTOR_FWD;
    leds_set(NONE);

    while(1)
    {
	if (battery_critical())
	{
	    low_power();
	}

	watchdog_update();

	/* Are we at the light source? */
	if(light_intensity == 0)
	{
		// Deposit food here (in original FW)
		leds_set(ORANGE);
		random_walk_disable();
		motor_r = motor_l = 6;
		motor_mode = MOTOR_FWD;
		time_wait(10);
	}

	/* Do we have a reasonable bearing? */
	else if(bearing_strength > 10)
	{
		leds_set(RED);
		random_walk_disable();
		braitenberg_update();
	}
	else
	{
		/* Random Walk */
		leds_set(GREEN);
		random_walk_enable();
	}
  }
}

void init(void)
{
	/* Disable the watchdog timer */
	WDTCTL = WDTHOLD | WDTPW;

	/* GPIO: All inputs */
	P1DIR = P2DIR = P3DIR = P4DIR = 0;

	/* Use a 16 MHz clock (DCO) */
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 &= ~0x0f;
	BCSCTL1 |= 
		/*XT2O=0: XT2 is on*/
		/*XTS=0: LFXT1 mode select. 0 -Low frequency mode*/
		DIVA_0 /* ACLK Divider 0: /1 */
		|CALBC1_16MHZ; /* BCSCTL1 Calibration Data for 16MHz */

	//BCSCTL2 = SELM_DCOCLK	/* MCLK from DCO */
	BCSCTL2 = SELM_0	/* MCLK from DCO */
		/* DIVMx = 0 : No MCLK divider */
		/* SELS = 0: SMCLK from DCO */
		/* DIVSx = 0 : No SMCLK divider */
		/* DCOR = 0 : DCO internal resistor */
		;

	BCSCTL3 = LFXT1S1; /*VLOCLK */

	//Init everything else
	flash_init();
	opamp1_init();
	bias_init();
	bias_bearing();
	adc10_init(); /* The order here matters. This configures the ADC */
	random_init(); /* Grab some random data */
	init_timera();
	init_timerb();
	motor_init();
	leds_init();
	battery_init();

	_EINT();
}

void low_power(void)
{
	/* Use as little power as possible */
	random_walk_disable();

	motor_off();
			
	/* IR Led off */
	P4OUT &= ~1;
	P4DIR |= 1;
	P4SEL &= ~1;
	
	_DINT();

	_BIS_SR(LPM3_bits);
	while(1);
}
