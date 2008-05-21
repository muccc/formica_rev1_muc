#include "device.h"
#include <signal.h>
#include "ir-rx.h"
#include "ir-tx.h"
#include "ir-bias.h"
#include "opamp-1.h"
#include "adc10.h"
#include "random.h"
#include "motor.h"
#include "leds.h"
#include "virus.h"
#include "net-rx.h"
#include "net-tx.h"
#include "battery.h"
#include "food.h"
#include "bearing.h"
#include "flash.h"

/* Initialises everything. */
void init(void);

int i = 0;

int main( void )
{
	init();
	while(1)
	{
		if(hasfood())
		{
			leds_red_on();
			uint16_t strength = bearing_get_strength();
			if(strength < 10)
			{
				motor_r = motor_l = 6;
				motor_mode = MOTOR_BK;
				uint32_t i;
				for(i=0;i<100000;i++);
			}
			else if(strength < 750)
			{
				leds_green_on();
				/* Braitenburg vehicle mode */
				random_walk_disable();
				uint16_t bearing = bearing_get();
				switch(bearing)
				{
					case 0:
						motor_mode = MOTOR_FWD;
						setmotorspeeds();
						break;
					case 120:
						motor_mode = MOTOR_TURN_RIGHT;
						motor_r = motor_l = 5;
						break;
					case 240:
						motor_mode = MOTOR_TURN_LEFT;
						motor_r = motor_l = 5;
						break;
					default:
						break;
				}
			}
			else
			{
				leds_green_off();
				/* Random Walk */
				motor_r = motor_l = 3;
				random_walk_enable();
			}
		}
		else
		{
			/* Not got food, just do random walk */
			leds_green_off();
			leds_red_off();
			motor_r = motor_l = 3;
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

	BCSCTL2 = SELM_DCOCLK	/* MCLK from DCO */
		/* DIVMx = 0 : No MCLK divider */
		/* SELS = 0: SMCLK from DCO */
		/* DIVSx = 0 : No SMCLK divider */
		/* DCOR = 0 : DCO internal resistor */;

	BCSCTL3 = LFXT1S1; /*VLOCLK */
	
	flash_init();
	opamp1_init();
	bias_init();
	bias_use1();

	adc10_init(); /* The order here matters. This configures the ADC */
	random_init(); /* Grab some random data */
	adc10_stream(); /* Prepare the ADC for streaming in data */
	
	net_rx_init();
	net_tx_init();
	ir_receive_init();
	ir_transmit_init();
	motor_init();
	leds_init();
	battery_init();
	food_init();

	eint();
	
	//virus_init();
}
