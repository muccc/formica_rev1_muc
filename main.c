#include "device.h"
#include <signal.h>
#include "ir-receive.h"
#include "ir-transmit.h"
#include "ir-bias.h"
#include "opamp-1.h"

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
	P1DIR = P2DIR = P3DIR = P4DIR = 0;

	/* Use a 16 MHz clock (DCO) */
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 &= ~0x0f;
	BCSCTL1 |= CALBC1_16MHZ;

	BCSCTL2 = SELM_DCOCLK	/* MCLK from DCO */
		/* DIVMx = 0 : No MCLK divider */
		/* SELS = 0: SMCLK from DCO */
		/* DIVSx = 0 : No SMCLK divider */
		/* DCOR = 0 : DCO internal resistor */;

	opamp1_init();
	bias_init();
	ir_receive_init();
	ir_transmit_init();

	eint();
}
