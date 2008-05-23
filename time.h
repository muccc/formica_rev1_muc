#ifndef __TIME_H
#define __TIME_H
#include <stdint.h>

extern volatile uint32_t the_time;

/* Pause for t ticks */
void time_wait( uint32_t t );

#endif	/* __TIME_H */
