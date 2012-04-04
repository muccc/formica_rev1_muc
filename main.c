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
#include "device.h"
#include <signal.h>
#include "ir-bias.h"
#include "opamp-1.h"
#include "adc10.h"
#include "random.h"
#include "motor.h"
#include "leds.h"
#include "flash.h"
#include "time.h"

#define PD_THRESHOLD 250

/* Initialises everything. */
void init(void);

int i = 0;
int loop = 0;

int main( void )
{
//        uint32_t chargeopportunity = 0;
   init();

//Switch on LED for 2sec in RED
    time_wait(TICKS_PER_SEC * 1);
    leds_set(RED);                // red LED on for 2 seconds
    P4DIR |= (0x09);                 //P40 and P43 are output IR
    P4OUT |= (0x09);            //all IR leds on (3 top, one bottom)
    time_wait(TICKS_PER_SEC * 2);

//Switch on LED for 2sec in GREEN
    leds_set(GREEN);          //green LED on for 1 second
    time_wait(TICKS_PER_SEC * 1);

//LEDs off
    leds_set(NONE);
	
    while(1)
    {
	//read three times adc to get all 3 light sensor values
	adc10_grab();
	leds_set(GREEN);
        time_wait(TICKS_PER_SEC *1);
	leds_set(NONE);
        time_wait(TICKS_PER_SEC *1);

	adc10_grab();
	leds_set(GREEN);
        time_wait(TICKS_PER_SEC *1);
	leds_set(NONE);
        time_wait(TICKS_PER_SEC *1);

	adc10_grab();
	leds_set(GREEN);
        time_wait(TICKS_PER_SEC *1);
	leds_set(NONE);
        time_wait(TICKS_PER_SEC *1);

        for (i=0; i<3; i++)
	{
		for (loop=0; loop<=i; loop++)
		{
			leds_set(ORANGE);
			time_wait(TICKS_PER_SEC/10);
			leds_set(NONE);
			time_wait(TICKS_PER_SEC/10);
		}
		for (loop=0; loop<pd_value[i]; loop++)
		{
			leds_set(RED);
			time_wait(TICKS_PER_SEC/10);
			leds_set(NONE);
			time_wait(TICKS_PER_SEC/10);
		}
		time_wait(TICKS_PER_SEC*1);
	}
	time_wait(TICKS_PER_SEC*3);	
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

	BCSCTL2 = SELM_DCOCLK	/* MCLK from DCO */
		/* DIVMx = 0 : No MCLK divider */
		/* SELS = 0: SMCLK from DCO */
		/* DIVSx = 0 : No SMCLK divider */
		/* DCOR = 0 : DCO internal resistor */;

	BCSCTL3 = LFXT1S1; /*VLOCLK */
	flash_init();
	opamp1_init();
	bias_init();
	motor_init();
	leds_init();
        adc10_init();	
	eint();
	
}
