#ifndef __FREQ_H
#define __FREQ_H

#define NBITS _NBITS
#define NFREQ _NFREQ
#define MIN_PERIOD _MIN_PERIOD
#define MAX_PERIOD _MAX_PERIOD

#define PERIOD_SPACING ((MAX_PERIOD - MIN_PERIOD)/(NFREQ-1))
#define RANGE (PERIOD_SPACING/2)

const uint16_t period_lut[_NFREQ];

#endif	/* __FREQ_H */
