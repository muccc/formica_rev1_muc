#ifndef __ADC_10_H
#define __ADC_10_H
#include <stdint.h>

void adc10_init( void );
uint16_t readtemp( void );
void streamadc( void );
void grabadc( void );
#endif	/* __ADC_10_H */
