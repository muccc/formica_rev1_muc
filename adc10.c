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
#include "adc10.h"
#include "ir-bias.h"
#include "device.h"
#include <signal.h>
#include <stdint.h>
#include "bearing.h"
//#include "ir-tx.h"
#include "leds.h"
#include "time.h"

/* Disable the ADC */
#define adc10_dis() do { ADC10CTL0 &= ~ENC; } while (0)

/* Select a channel (0 <= x <= 15) */
#define adc10_set_channel(x) do { ADC10CTL1 &= ~INCH_15;	\
		ADC10CTL1 |= x << 12; } while (0)

uint16_t pd_value[3];

static enum {
	PD1,
	PD2,
	PD3,
} curreading = PD1;

#define PD1_CHANNEL 1
#define PD2_CHANNEL 2
#define PD3_CHANNEL 3

#define CHANNEL_CONFIG (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<7)


void adc10_init( void )
{
	ADC10CTL0 = SREF_0 	/* Use VCC and VSS as the references */
		| ADC10SHT_DIV64 /* 64 x ADC10CLKs
				    32 us */
		/* ADC10SR = 0 -- Support 200 ksps sampling (TODO: maybe this can be set) */
		/* REFOUT = 0 -- Reference output off */
		/* REFBURST = 0 -- Reference buffer on continuously (TODO) */
		//	| MSC		/* Move onto the next conversion after the previous*/
		| REF2_5V
		| REFON         /* Use 2.5V reference */
		| ADC10ON	/* Peripheral on */
	        | ADC10IE;       /* Interrupt enabled */
	//  | ENC; 		/* ADC Enabled */

	ADC10CTL1 = /* Select the channel later... */
		SHS_0		/* ADC10SC is the sample-and-hold selector */
		/* ADC10DF = 0 -- Straight binary format */
		/* ISSH = 0 -- No inversion on the s&h signal */
		| ADC10DIV_7	/* Divide clock by 8 */
		| ADC10SSEL_MCLK
		| CONSEQ_0; 	/* Single channel single conversion */
		

	/* PD1, PD2, PD3 and FOOD are on P2.1, P2.2, P2.3 and P2.4 respectively */
	/* P2.1, P2.2, P2.3, P2.4 (page 60 of the MSP430F2234 datasheet) */
	/* RX is on P3.7 (A7) */
	ADC10AE0 = CHANNEL_CONFIG;
	/* Enable A15 (Batt) */
	ADC10AE1 = 0x80;

	ADC10DTC0 |= ADC10CT; /* DTC Not used. This makes it continuous */
	
	adc10_set_channel(PD1_CHANNEL);
}


void adc10_grab( void )
{
	/* Ignore request for ADC reading if there's one already happening */
	if( ADC10CTL1 & ADC10BUSY )
		return;

	bias_bearing();
	/* Start the conversion: */
	ADC10CTL0 |= (ADC10SC | ENC);
	//}
}


interrupt (ADC10_VECTOR) adc10_isr( void )
{
	/* back to IR reception bias */
	adc10_dis();

	switch(curreading){
	case PD1:
		pd_value[0] = ADC10MEM;

		adc10_set_channel(PD2_CHANNEL);
		curreading = PD2;
		break;
	case PD2:
		pd_value[1] = ADC10MEM;

		adc10_set_channel(PD3_CHANNEL);
		curreading = PD3;
		break;
	case PD3:
		pd_value[2] = ADC10MEM;
                
		adc10_set_channel(PD1_CHANNEL);
		curreading = PD1;
		break;
	}
}
