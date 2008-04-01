#include "timer-a.h"
#include "device.h"
#include <signal.h>

#define timera_en() do { TACTL |= MC_UPTO_CCR0; } while(0)

void timera_init( void )
{
	TACTL = TASSEL_SMCLK	/* SMCLK clock source */
		/* No clock divider */
		| MC_STOP	/* Timer stopped for now */
		/* Don't reset the timer */
		| TAIE		/* Interrupt enabled */;

	/* Clear the timer */
	TAR = 0;

	/* 50 us between interrupts (800 clock ticks) */
	TACCR0 = 8000;

	/* Set to output */
	P1DIR |= 1;

	/* Start the timer */
	timera_en();
}

interrupt (TIMERA1_VECTOR) timer_a_isr(void)
{
	P1OUT ^= 1;
	
	/* Clear the interrupt flag */
	TACTL &= ~TAIFG;
}

interrupt (TIMERA0_VECTOR) timer_a1_isr(void)
{
	nop();
}
