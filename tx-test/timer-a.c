#include "timer-a.h"
#include "device.h"
#include "../freq.h"
#include "net.h"
#include "types.h"
#include <string.h>
#include <signal.h>

#define SYM_MASK ((1 << NBITS)-1)

#define timera_en() do { TACTL |= MC_UPTO_CCR0; } while(0)

/* The current symbol that's being transmitted */
static uint8_t tx_sym = 0;

/* Returns the next symbol to transmit */
static uint8_t next_symbol( void );

/* Returns the next byte of data to transmit */
static uint8_t next_byte( void );

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

	if( cycle == 14 ) {
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
	static uint8_t curbyte;
	/* How many symbols have been transmitted from the current byte */
	static uint8_t cb_pos = 4;
	static uint8_t last_sym;

#if NBITS != 3
#error Cannot cope with NBITS not being 3
#endif

	if( cb_pos == 4 ) {
		/* Send the start byte symbol */
		sym = 0;
		cb_pos = 0;
		curbyte = next_byte();

	} else {
		/* Grab the data from the byte */
		sym = curbyte & SYM_MASK;
		curbyte >>= NBITS;
			
		/* Now munge up the symbol so that we don't 
		   transmit the same one again! */
		
		/* First, add 1 because symbol 0 is the 'start bit' symbol */
		sym++;

		/* Avoid the symbol that was transmitted last */
		if( sym >= last_sym )
			sym++;
	}
	cb_pos++;

	last_sym = sym;
	return sym;
}

static uint8_t next_byte( void )
{
	/* The current packet that's being transmitted */
	static const uint8_t *cur_packet = NULL;
	/* The length of the current packet */
	static uint8_t cp_len = 0;

	if( cur_packet == NULL )
		cur_packet = net_get_next_packet( &cp_len );

	if( cur_packet != NULL ) {
		static uint8_t p = 0;
		static uint8_t checksum = 0;
		static bool escaped = FALSE;

		uint8_t b;

		if( p == 0 ) {
			/* Send the 'start of frame' byte */
			b = 0x7E;
			checksum = 0;

			P1OUT |= 2;
		}
		else {
			if( p == 1 ) {
				b = cp_len;
				if( !escaped )
					checksum += b;
			}
			else if( p < (cp_len + 2) ) {
				b = cur_packet[ p - 2 ];
				if( !escaped )
					checksum += b;
			}
			else
				/* The checksum */
				b = 0xff - checksum;

			if( !escaped && (b == 0x7e || b == 0x7d) ) {
				escaped = TRUE;
				b = 0x7d;
			} 
			else if( escaped ) {
				escaped = FALSE;
				b ^= 0x20;
			}
		}

		if( !escaped ) {
			if( p == cp_len + 2 )
			{
				p = 0;
				cur_packet = net_get_next_packet( &cp_len );
			}
			else
				p++;
		}

		return b;
	}
	else
		return 0;
}
