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
uint8_t pd_loop = 0;

/*
static enum {
	PD1,
	PD2,
	PD3,
} curreading = PD1;
*/

#define PD1_CHANNEL 1
#define PD2_CHANNEL 2
#define PD3_CHANNEL 3

#define CHANNEL_CONFIG (1<<1) | (1<<2) | (1<<3)


void adc10_init( void )
{
	ADC10CTL1 = INCH_2 + CONSEQ_1;            // A2/A1/A0, once multi channel
	ADC10CTL1 &= ~ADC10DF; //Straight binary format
	ADC10CTL0 = REF2_5V + REFON + ADC10SHT_2 + MSC + ADC10ON;
  	ADC10DTC1 = 0x03;                         // 3 conversion	

	/* PD1, PD2, PD3 and FOOD are on P2.1, P2.2, P2.3 and P2.4 respectively */
	/* P2.1, P2.2, P2.3, P2.4 (page 60 of the MSP430F2234 datasheet) */
	/* RX is on P3.7 (A7) */
	//ADC10AE0 = 0x0E;
	ADC10AE0 = BIT1 + BIT2 + BIT3;
}


void adc10_grab( void )
{

	ADC10SA = (uint16_t)pd_value; // Data buffer start
        ADC10CTL0 |= ENC + ADC10SC; // Start the conversion
	//Wait until conversion is ready
	while( ADC10CTL1 & ADC10BUSY ){}
}

