#ifndef __TIME_H
#define __TIME_H
#include <stdint.h>

extern uint32_t the_time;

/* Pause for t ticks */
void time_wait( uint16_t t );

#endif	/* __TIME_H */
