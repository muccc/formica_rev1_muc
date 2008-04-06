#include "timer-a.h"
#include "device.h"
#include <signal.h>

#define timera_en() do { TACTL |= MC_CONT; } while(0)

void timera_init( void )
{
	TACTL = TASSEL_SMCLK	/* SMCLK clock source */
		/* No clock divider */
		| MC_STOP	/* Timer stopped for now */
		/* Don't reset the timer */
		| TAIE		/* Interrupt enabled */;

	/* Clear the timer */
	TAR = 0;

	/* Set to output */
	P1DIR |= 3;

	/* Set up CCR1 to trigger off the comparator */
	TACCTL1 = CM_POS	/* Trigger on rising edge */
		| CCIS_1	/* CCI1B trigger source (comes from the comparator) */
		| SCS		/* Synchronize with timer clock */
		| CAP		/* Capture mode */
		| CCIE;		/* Interrupt enabled */

	/* Start the timer */
	timera_en();
}

interrupt (TIMERA1_VECTOR) timer_a_isr(void)
{
	uint16_t taiv_l = TAIV;

	switch( taiv_l )
	{
		/* CCR1: */
	case 0x02: {
		static uint16_t last = 0;
		uint16_t reading = TACCR1;

		if( reading > last )
			reading -= last;
		else
			reading = ((uint32_t)(reading) + 0xffff) - last;

		if( reading < 300 )
			P1OUT = 1;
		else
			P1OUT = 0;

		last = TACCR1;
		break;
	}

		/* Timer overflow */
	case 0x0A: {
		P1OUT ^= 1;

		break;
	}

	default:
		/* Should never get here. */
		nop();
	}

}

interrupt (TIMERA0_VECTOR) timer_a1_isr(void)
{

}
