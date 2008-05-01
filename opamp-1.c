#include "opamp-1.h"
#include "device.h"


void opamp1_init( void )
{
	OA1CTL0 = /* OANx = Don't care */
		OAP_2		/* OA1I2 to non-inverting input */
		| OAPM_3//OAPM_3	/* Fast slew rate */
		| OAADC0;	/* OA1OUT to external pins (and some ADC inputs) */

	OA1CTL1 =  OAFBR_2	/* Tap 2 - 8R/8R */
		| OAFC_3	/* Comparator mode */
		/* OANEXT = 0 -- inverting input not externally available */
		| OARRIP;

	/* Put the output on P4.4 */
	ADC10AE |= 0x20;

	/* Configure the input pins */
	/* RX is connected to P3.7 (OA1I2) */
	/* See page 70 of the MSP430F2234 datasheet */
	ADC10AE |= 0x80;

	/* RX is also connected to P2.0 */
	/* We don't care about this pin, but to reduce current, we'll route it to A0 */
	/* Page 60 of the MSP430F2234 datasheet */
	ADC10AE |= 1;
}
