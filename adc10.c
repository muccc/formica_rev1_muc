#include "adc10.h"
#include "ir-bias.h"
#include "device.h"
#include <signal.h>
#include <stdint.h>
#include "food.h"

void adc10_init( void )
{
	ADC10CTL0 = SREF_0 	/* Use VCC and VSS as the references */
		| ADC10SHT_DIV4 /* 4 x ADC10CLKs */
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
		| ADC10SSEL_ACLK
		| CONSEQ_0; 	/* Single channel single conversion */
		

	/* PD1, PD2, PD3 and FOOD are on P2.1, P2.2, P2.3 and P2.4 respectively */
	/* P2.1, P2.2, P2.3, P2.4 (page 60 of the MSP430F2234 datasheet) */
	ADC10AE |= (1<<1) | (1<<2) | (1<<3) | (1<<4);
	ADC10DTC0 |= ADC10CT; /*Data is transferred continuously. DTC operation is stopped only if
							ADC10CT cleared, or ADC10SA is written to.*/
	//ADC10SA = &pointer to some array
	
	ADC10CTL1 &= ~INCH_15; /*Clearing the channel selection*/
	ADC10CTL1 |= INCH_A4; /*Food sensor*/
	/* Start the conversion: */
	ADC10CTL0 |= (ENC | ADC10SC);
	
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

uint16_t a3data; /*output from PD3*/
uint16_t a2data; /*output from PD2*/
uint16_t a1data; /*output from PD1*/
interrupt (ADC10_VECTOR) adc10_isr( void )
{
	switch(ADC10CTL1 & 0xF<< 12){ /*Reading the current sampling channel, IS THIS SANE?*/
		case INCH_A1:
			a1data = ADC10MEM;
			/*Disable the ADC*/
			ADC10CTL0 &= ~ENC;
			ADC10CTL1 &= ~INCH_15; /*Clearing the channel selection*/
			ADC10CTL1 |= INCH_A4; /*Food sensor*/		
			break;
		case INCH_A2:
			a2data = ADC10MEM;
			/*Disable the ADC*/
			ADC10CTL0 &= ~ENC;
			ADC10CTL1 &= ~INCH_15; /*Clearing the channel selection*/
			ADC10CTL1 |= INCH_A1; /*Food sensor*/		
			break;
		case INCH_A3:
			a3data = ADC10MEM;
			/*Disable the ADC*/
			ADC10CTL0 &= ~ENC;
			ADC10CTL1 &= ~INCH_15; /*Clearing the channel selection*/
			ADC10CTL1 |= INCH_A2; /*Food sensor*/			
			break;
		case INCH_A4:
			fooddata = ADC10MEM;
			if(P2DIR & BIAS1){
			/*Disable the ADC*/
				ADC10CTL0 &= ~ENC;
				ADC10CTL1 &= ~INCH_15; /*Clearing the channel selection*/
				ADC10CTL1 |= INCH_A3; /*Food sensor*/
			}
			break;
		default:
			break;
	}
	ADC10CTL0 |= (ENC | ADC10SC);
}
