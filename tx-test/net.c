#include "net.h"
#include <string.h>

const uint8_t *p0 = "hello";
const uint8_t p1[] = {0x1,0x2,0x7e,0x7d,0x20,0x00};

const uint8_t* net_get_next_packet( uint8_t *len )
{
	static uint8_t p = 0;

	if( p == 0 ) {
		p = 1;
		*len = strlen(p0);
		return p0;
	} else {
		p = 0;
		*len = 6;
		return p1;
	}
}
