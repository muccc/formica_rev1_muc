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
		/* Previous periods */
		static int16_t prev[] = {0xffff,0,0xffff};
		/* Position in the prev buffer */
		static uint8_t pos = 0;
		static uint16_t last_period = 0;
		uint16_t reading = TACCR1;

		{
			/* The last TACCR1 reading: */
			static uint16_t last = 0;

			/* The timer may have wrapped around since the last event */
			/* Work out the ticks since the last event happened */
			if( reading > last )
				reading -= last;
			else
				reading = ((uint32_t)(reading) + 0xffff) - last;
			last = TACCR1;

			prev[pos] = reading - last_period;
			if( pos == 2 )
				pos = 0;
			else
				pos ++;

			last_period = reading;
		}

		{
			uint32_t sum = 0;
			uint8_t i;

			for(i=0; i<3; i++)
				if( prev[i] < 0 )
					sum += (-prev[i]);
				else
					sum += prev[i];

			if( sum < 400 )
			{
				/* Got a stable frequency */
				P1OUT &= ~0x10;
#define SPACE 500
				if( (2000-SPACE) < reading && reading < (2000+SPACE) )
					P1OUT |= 1;
				else if( (4000-SPACE) < reading && reading < (4000+SPACE) )
					P1OUT &= ~1;
			} else {
				/* Unstable frequency */
				P1OUT |= 0x10;
			}	
		}
		break;
	}

		/* Timer overflow */
	case 0x0A: {

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
