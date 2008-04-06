#include "device.h"
#include <signal.h>
#include "timer-a.h"
#include "comp.h"

/* Initialises everything. */
void init(void);

int i = 0;

int main( void )
{
	init();

	while(1);
}

void init(void)
{
	/* Disable the watchdog timer */
	WDTCTL = WDTHOLD | WDTPW;

	/* GPIO: All inputs */
	P1DIR = P2DIR = 0;

	P1DIR |= 0x10;

	/* Use a 16 MHz clock (DCO) */
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 &= ~0x0f;
	BCSCTL1 |= CALBC1_16MHZ;

	BCSCTL2 = SELM_DCOCLK	/* MCLK from DCO */
		/* DIVMx = 0 : No MCLK divider */
		/* SELS = 0: SMCLK from DCO */
		/* DIVSx = 0 : No SMCLK divider */
		/* DCOR = 0 : DCO internal resistor */;

	comp_init();
	timera_init();

	eint();
}
