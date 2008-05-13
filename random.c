#include "random.h"
#include "adc10.h"
#include "device.h"
#include <signal.h>

uint16_t lfsr;

void random_init( void )
{
    /* Seed with a temperature reading */
    lfsr = readtemp();

    /*Spin a few times to spice it up a bit*/
    random();
    random();
    random();
}

uint16_t random( void )
{
    /* Rotate the 10 bit LFSR */
    lfsr = (lfsr >> 1) | ((uint16_t)((lfsr & 0x09) ^
                                     (lfsr & 0x06) ^
                                      1) << 0x09);
    return lfsr;
}
