#ifndef __SMBUS_PEC_H
#define __SMBUS_PEC_H
#include <stdint.h>

#define checksum_add(C,B) do { C = crc8( C ^ B ); } while (0)

uint8_t crc8( uint8_t tempdata );

#endif	/* __SMBUS_PEC_H */
