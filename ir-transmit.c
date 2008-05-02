#include "ir-transmit.h"
#include "device.h"
#include <signal.h>
#include "freq.h"
#include "ir-tx-data.h"

/* Puts the timer into stop mode */
#define timer_b_dis() do { TBCTL &= ~MC_3; } while (0)
/* Puts the timer into up mode */
#define timer_b_en() do { TBCTL |= MC_UPTO_CCR0; } while (0)

void ir_transmit_init( void )
{
	/* Configure timer B to perform the transmission */
	TBCTL = TBCLGRP_0 	/* Each TBCLx latch loads independently */
		| CNTL_0	/* 16-bit mode */
		| TBSSEL_SMCLK	/* Source Timer B from SMCLK */
		| ID_DIV1 	/* Divide by 1 */
		| MC_STOP;	/* Stopped for now */
		/* Interrupt disabled */

	/* CCR0 generates the IR TX signal */
	TBCCTL0 = CM_DISABLE 	/* No capture */
		| CCIS_2	/* GND */
		| SCS		/* Synchronize with the timer clock*/
		| CLLD_0	/* Load TBCL0 when TBCCR0 is written */
				/* (We only do this for the first write) */
		/* Compare mode */
		| OUTMOD_TOGGLE	/* Toggle the output when the compare matches */
		| CCIE;		/* Enable interrupt */

	/* TX is on pin P4.0 (TB0) */
	/* Connect TB0 to it */
	/* See page 71 of the MSP430F2234 datasheet: */
	P4DIR |= 1;
	P4SEL |= 1;
	
	TBCCR0 = 8000;
	/* Now only load TBCCR0 when TBR counts to 0 */
	TBCCTL0 |= CLLD_1;

	timer_b_en();
}

interrupt (TIMERB0_VECTOR) timer_b_isr(void)
{
	/* Which period we're in */
	static uint8_t period = 0;
	uint8_t sym;

	/* Only change output period during the low section */
	if( P4IN & 1 )
		return;

	period++;
	/* Transmit 3 periods of each frequency */
	if( period != 4 )
		return;
	period = 0;

	sym = ir_tx_next_symbol();
	TBCCR0 = period_lut[ sym ];

	/* Flag is automatically cleared */
}

interrupt (TIMERB1_VECTOR) timer_b_isr2(void)
{
	/* This interrupt should never happen */
}

