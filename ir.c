#include "ir.h"
#include "ir-tx.h"
#include "ir-bias.h"
#include "net-tx.h"
#include "leds.h"
#include "adc10.h"
#include <stdint.h>

void ir_nudge( void )
{
	static uint16_t ir_count = 0; /* Time spent receiving */
	static uint16_t ir_tx_time = 1500; /* Time spent transmitting */

	if( ir_transmit_is_enabled() ) {
		ir_tx_time += 4;
	}
	else
	{
		if( ir_count == ir_tx_time )
		{
			/* Finished receiving, start transmit cycle */
			ir_tx_time = 0;
			ir_count = 0;
			net_tx_enable_for(TX_AT_A_TIME);
		}
		else
			/* Continue receiving */
			ir_count++;
	}
}
