#ifndef __BEARING_H
#define __BEARING_H

/* Set the bearing with new ADC readings.
   Arguments: 
     - a,b,c: Photodiode ADC readings */
void bearing_set(uint16_t a, uint16_t b, uint16_t c);

/* Get the bearing (0-359). */
#define bearing_get() ( bearing )

/* Get the maximum measured light intensity (0-1023) */
#define bearing_get_strength() ( bearing_strength )

void setmotorspeeds( void );

extern uint16_t bearing;
extern uint16_t bearing_strength;

#endif
