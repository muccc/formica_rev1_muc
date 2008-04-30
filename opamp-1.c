#include "opamp-1.h"
#include "device.h"


void opamp1_init( void )
{
	OA1CTL0 = /* OANx = Don't care */
		OAP_2		/* OA1I2 to non-inverting input */
		| OAPM_3	/* Fast slew rate */
		| OAADC0;	/* OA1OUT to external pins (and some ADC inputs) */

	OA1CTL1 =  OAFBR_2	/* Tap 2 - 8R/8R */
		| OAFC_3	/* Comparator mode */
		/* OANEXT = 0 -- inverting input not externally available */
		| OARRIP;

	/* Put the output on P4.4 */
	ADC10AE |= 0x20;
}
