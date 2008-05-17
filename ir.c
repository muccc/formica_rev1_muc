#include "ir.h"
#include "ir-tx.h"
#include "net-tx.h"
#include "leds.h"
#include <stdint.h>

void ir_nudge( void )
{
	static uint16_t ir_count = 0;
	static uint16_t ir_tx_time = 1500;

	if( ir_transmit_is_enabled() ) {
		ir_tx_time++;
	}
	else
	{
		if( ir_count == ir_tx_time )
		{
			ir_tx_time = 0;
			ir_count = 0;
			net_tx_enable_for(5);
		}
		else
			ir_count++;
	}
}
