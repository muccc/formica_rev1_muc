#include "random.h"
#include "adc10.h"
#include "device.h"
#include <signal.h>

uint16_t lfsr;

void random_init( void )
{
	/* Seed with a temperature reading */
	lfsr = adc10_readtemp() & (~0x03ff);

	/*Spin a few times to spice it up a bit*/
	random();
	random();
	random();
}

uint16_t random( void )
{
	/* Rotate the 10 bit LFSR */
	/* Bit 9 xor Bit 6 xor 1 */
	lfsr = (lfsr >> 1) | ((uint16_t)( ((lfsr & 0x0200) >> 9) ^
					  ((lfsr & 0x0040) >> 6) ^
					 1) << 0x09);
	return lfsr;
}
