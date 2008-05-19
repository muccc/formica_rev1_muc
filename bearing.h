#ifndef __BEARING_H
#define __BEARING_H

/* Set the values, no calculation done, very fast */
void setbearing(uint16_t a, uint16_t b, uint16_t c);
/* Get the current bearing. Sums done at this stage */
uint16_t getbearing( void );
#endif
