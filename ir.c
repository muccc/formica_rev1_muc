/*  Copyright 2008 Stephen English, Jeffrey Gough, Alexis Johnson, 
        Robert Spanton and Joanna A. Sun.

    This file is part of the Formica robot firmware.

    The Formica robot firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Formica robot firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Formica robot firmware.  
    If not, see <http://www.gnu.org/licenses/>.  */
#include "ir.h"
//#include "ir-tx.h"
#include "ir-bias.h"
//#include "net-tx.h"
#include "leds.h"
#include "adc10.h"
#include <stdint.h>

void ir_nudge( void )
{
	static uint16_t ir_count = 0; /* Time spent receiving */
	static uint16_t ir_tx_time = 1500; /* Time spent transmitting */

		if( ir_count == ir_tx_time )
		{
			/* Finished receiving, start transmit cycle */
			ir_tx_time = 0;
			ir_count = 0;
		//	net_tx_enable_for(TX_AT_A_TIME);
		}
		else
			/* Continue receiving */
			ir_count++;
}
