#include "adc10.h"
#include "ir-bias.h"
#include "device.h"
#include <signal.h>
#include <stdint.h>
#include "food.h"
#include "bearing.h"

/* Disable the ADC */
#define adc10_dis() do { ADC10CTL0 &= ~ENC; } while (0)

uint16_t pd_value[3];

static enum {
	PD1,
	PD2,
	PD3,
	FOOD0,
	FOOD1
} curreading = PD1;

void adc10_init( void )
{
	ADC10CTL0 = SREF_0 	/* Use VCC and VSS as the references */
		| ADC10SHT_DIV64 /* 64 x ADC10CLKs
				    32us*/
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
		| ADC10SSEL_MCLK
		| CONSEQ_0; 	/* Single channel single conversion */
		

	/* PD1, PD2, PD3 and FOOD are on P2.1, P2.2, P2.3 and P2.4 respectively */
	/* P2.1, P2.2, P2.3, P2.4 (page 60 of the MSP430F2234 datasheet) */
	ADC10AE |= (1<<1) | (1<<2) | (1<<3) | (1<<4);
	ADC10DTC0 |= ADC10CT; /* DTC Not used. This makes it continuous */
}

void adc10_stream( void )
{
	ADC10CTL1 &= ~INCH_15; /*Clearing the channel selection*/
	ADC10CTL1 |= INCH_A1; /* Start with sunlight 1 */
	curreading = PD1;
}

void adc10_grab( void )
{
	/* Start the conversion: */
	ADC10CTL0 |= (ENC | ADC10SC);
	if(curreading == FOOD1)
		fled_on();
	else
		bias_use2();
}

uint16_t adc10_readtemp( void )
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
	adc10_dis();
	return boottemp;
}

interrupt (ADC10_VECTOR) adc10_isr( void )
{
	static uint16_t food0; /*output from food with LED off*/
	static uint16_t food1; /*output from food with LED on*/

	switch(curreading){
		case PD1:
			pd_value[0] = ADC10MEM;

			adc10_dis();
			ADC10CTL1 &= ~INCH_15; /*Clearing the channel selection*/
			ADC10CTL1 |= INCH_A2;
			curreading = PD2;
			break;
		case PD2:
			pd_value[1] = ADC10MEM;

			adc10_dis();
			ADC10CTL1 &= ~INCH_15; /*Clearing the channel selection*/
			ADC10CTL1 |= INCH_A3;
			curreading = PD3;
			break;
		case PD3:
			pd_value[2] = ADC10MEM;

			adc10_dis();
			ADC10CTL1 &= ~INCH_15; /*Clearing the channel selection*/
			ADC10CTL1 |= INCH_A4;
			
			bearing_set( pd_value );

			curreading = FOOD0;
			break;
		case FOOD0:
			/* FLED Off */
			food0 = ADC10MEM;

			adc10_dis();
			ADC10CTL1 &= ~INCH_15; /*Clearing the channel selection*/
			ADC10CTL1 |= INCH_A4;

			curreading = FOOD1;
			break;
		case FOOD1:
			/* Fled ON */
			food1 = ADC10MEM;

			adc10_dis();
			ADC10CTL1 &= ~INCH_15; /*Clearing the channel selection*/
			ADC10CTL1 |= INCH_A1;

			foodcallback(food0, food1);
			
			curreading = PD1;
			break;
	}
	bias_use1();
	fled_off();
}
