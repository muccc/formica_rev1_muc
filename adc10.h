#ifndef __ADC_10_H
#define __ADC_10_H
#include <stdint.h>

void adc10_init( void ); /* Standard init function */
uint16_t readtemp( void ); /* Read the temperature. Must be after _init whilst ADC is disabled*/
void streamadc( void ); /* Configure the ADC ready to start streaming out in TX downtime */
void grabadc( void ); /* Grab a single reading */
#endif	/* __ADC_10_H */
