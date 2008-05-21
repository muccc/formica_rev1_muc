#ifndef __BEARING_H
#define __BEARING_H

/* Set the bearing with new ADC readings.
   Arguments: 
     - a,b,c: Photodiode ADC readings */
void bearing_set(uint16_t a, uint16_t b, uint16_t c);

/* Get the bearing (0-359). */
uint16_t bearing_get( void );

/* Get the maximum measured light intensity (0-1023) */
uint16_t bearing_get_strength( void );

void setmotorspeeds( void );

#endif
