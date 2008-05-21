#ifndef __ADC_10_H
#define __ADC_10_H
#include <stdint.h>

/* Standard init function */
void adc10_init( void );

/* Read the temperature. Must be after _init whilst ADC is disabled*/
uint16_t adc10_readtemp( void );

/* Configure the ADC ready to start streaming out in TX downtime */
void adc10_stream( void );

/* Grab a single reading */
void adc10_grab( void );
#endif	/* __ADC_10_H */
