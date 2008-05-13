#include "adc10.h"
#include "ir-bias.h"
#include "device.h"
#include <signal.h>
#include <stdint.h>

void adc10_init( void )
{
	ADC10CTL0 = SREF_0 	/* Use VCC and VSS as the references */
		| ADC10SHT_DIV64 /* Longest sample-and-hold time */
		/* ADC10SR = 0 -- Support 200 ksps sampling (TODO: maybe this can be set) */
		/* REFOUT = 0 -- Reference output off */
		/* REFBURST = 0 -- Reference buffer on continuously (TODO) */
		| MSC		/* Move onto the next conversion after the previous*/
		/* REF2_5V = REFON = 0 -- Not using internal reference */
		| ADC10ON	/* Peripheral on */
		| ADC10IE;	/* Interrupt enabled */

	ADC10CTL1 = /* Select the channel later... */
		SHS_0		/* ADC10SC is the sample-and-hold selector */
		/* ADC10DF = 0 -- Straight binary format */
		/* ISSH = 0 -- No inversion on the s&h signal */
		| ADC10DIV_7	/* Divide clock by 8 */
		| ADC10SSEL_SMCLK
		| CONSEQ_0; 	/* Signal-channel single-conversion mode */
		

	/* PD1, PD2 and PD3 are on P2.1, P2.2 and P2.3 respectively */
	/* P2.1, P2.2, P2.3 (page 60 of the MSP430F2234 datasheet) */
	ADC10AE |= (1<<1) | (1<<2) | (1<<3);

/* 	bias_use2(); */
}

uint16_t readtemp( void )
{
    uint16_t boottemp;

    /* If the ADC is already enabled return 0 */
    if(ADC10CTL0 & ENC)
        return 0;

    /* Read the temperature to initialise a random number generator */
    ADC10CTL1 &= ~INCH_15;
    ADC10CTL1 |= INCH_TEMP; /*Temperature sensor*/

 	/* Start the conversion: */
 	ADC10CTL0 |= (ENC | ADC10SC);

    /* Wait for the conversion to finish */
    while(!(ADC10CTL0 & ADC10IFG));
    boottemp = ADC10MEM;

    /*Disable the ADC*/
    ADC10CTL0 &= ~ENC;
    return boottemp;
}

interrupt (ADC10_VECTOR) adc10_isr( void )
{
	nop();
	ADC10CTL0 |= (ENC | ADC10SC);
}
