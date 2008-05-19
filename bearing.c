#include "device.h"

/* PD1 Straight ahead */
/* PD2 at 120' */
/* PD3 at 240' */
uint16_t pd1, pd2, pd3;

void setbearing(uint16_t a, uint16_t b, uint16_t c)
{
	pd1 = a; pd2 = b; pd3 = c;
}

uint16_t getbearing( void )
{
}
