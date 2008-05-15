#include "smbus_pec.h"

#define POLY    (0x1070U << 3)

uint8_t crc8( uint8_t tempdata )
{
	uint8_t i;
	uint16_t data;

	data = (uint16_t)tempdata<<8;
	for(i = 0; i < 8; i++) 
	{
		if (data & 0x8000)
			data = data ^ POLY;
		data <<= 1;
	}

	return (data >> 8) & 0xFF;
}
