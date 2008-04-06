#include "comp.h"
#include "device.h"

void comp_init( void )
{
	/* Switch CA2 and CA6 over to the comparator */
	P1SEL |= (1 << 2) | (1 << 6);

	CACTL1 = /* CAEX = 0  - don't exchange inputs */
		/* CARSEL = 0 - VCAREF connected to + terminal */
		CAREF_050	/* Half VCC reference */
		| CAON		/* Comparator turned on */
		/* CAIES = 0 - Interrupt on falling edge */
		/* CAIE = 0 - Interrupt disabled */;

	CACTL2 = /* CASHORT = 0 - Inputs not shorted */
		/* CA6 is the input */
		P2CA3 | P2CA2 
		/* CA2 outputs the reference for the moment */
		| P2CA4 | P2CA0
		/* CAF = 0 - No output filtering */;

	/* Disable the digital input on CA2 and CA7 */
	CAPD = CAPD2 | CAPD6;

	/* Get the comparator output on P1.3: */
	P1DIR |= 1<<3;
	P1SEL |= 1<<3;
}
