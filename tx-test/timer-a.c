#include "timer-a.h"
#include "device.h"
#include "../freq.h"
#include <signal.h>

#define timera_en() do { TACTL |= MC_UPTO_CCR0; } while(0)

/* The current symbol that's being transmitted */
static uint8_t tx_sym = 0;

static uint8_t data = 0x5A;
static uint8_t data_pos = 0;

/* Returns the next symbol to transmit */
static uint8_t next_symbol( void );

void timera_init( void )
{
	TACTL = TASSEL_SMCLK	/* SMCLK clock source */
		/* No clock divider */
		| MC_STOP	/* Timer stopped for now */
		/* Don't reset the timer */
		| TAIE		/* Interrupt enabled */;

	/* Clear the timer */
	TAR = 0;

	tx_sym = NFREQ-1;
	TACCR0 = period_lut[tx_sym];

	/* Set to output */
	P1DIR |= 3;

	/* Start the timer */
	timera_en();
}

interrupt (TIMERA1_VECTOR) timer_a_isr(void)
{
	static uint16_t cycle = 0;
	P1OUT &= ~2;

	if( cycle == 20 ) {
		tx_sym = next_symbol();

		TACCR0 = period_lut[tx_sym];

		cycle = 0;
	} else
		cycle++;

	P1OUT ^= 1;

	/* Clear the interrupt flag */
	TACTL &= ~TAIFG;
}

interrupt (TIMERA0_VECTOR) timer_a1_isr(void)
{
	nop();
}

static uint8_t next_symbol( void )
{
	uint8_t sym;

	if( tx_sym == 0 )
		P1OUT |= 2;

	sym = tx_sym + 1;
	if( sym == NFREQ )
		sym = 0;
	return sym;
}
