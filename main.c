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
#include "battery.h"

/* Initialises everything. */
void init(void);

int i = 0;

int main( void )
{
	init();

	while(1)
	{
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
		DIVA_3 /* ACLK Divider 3: /8 */
		|CALBC1_16MHZ; /* BCSCTL1 Calibration Data for 16MHz */

	BCSCTL2 = SELM_DCOCLK	/* MCLK from DCO */
		/* DIVMx = 0 : No MCLK divider */
		/* SELS = 0: SMCLK from DCO */
		/* DIVSx = 0 : No SMCLK divider */
		/* DCOR = 0 : DCO internal resistor */;

	BCSCTL3 = LFXT1S1; /*VLOCLK */
	
	opamp1_init();
	bias_init();
	adc10_init();
	random_init();
	net_rx_init();
	ir_receive_init();
	ir_transmit_init();
	motor_init();
	leds_init();
	battery_init();

	eint();

	virus_init();
}
