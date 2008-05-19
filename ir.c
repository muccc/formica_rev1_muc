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
		/* Continue transmitting */
		ir_tx_time++;
	}
	else
	{
		if( ir_count == ir_tx_time )
		{
			/* Finished receiving, start transmit cycle */
			ir_tx_time = 0;
			ir_count = 0;
			net_tx_enable_for(5);
		}
		else
			/* Continue receiving */
			ir_count++;
	}
}
