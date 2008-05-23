#ifndef __BEARING_H
#define __BEARING_H

/* Set the bearing with new ADC readings.
   Argument:
   - pdr: Array of photodiode readings (length 3). */
void bearing_set( uint16_t* pdr );

/* Get the bearing (0-359). */
#define bearing_get() ( bearing )

/* Get the maximum measured light intensity (0-1023) */
#define bearing_get_strength() ( bearing_strength )

extern uint16_t bearing;
extern uint16_t bearing_strength;
extern uint16_t light_intensity;

#endif
