#ifndef __FLASH_H
#define __FLASH_H
#include <stdint.h>

/* WARNING: These are highly MSP430F2254 specific: */
#define AREA_0 0xc000
#define AREA_1 0xe000
#define AREA_LEN 0x1e00

#define IVT ((uint16_t*)0xFFC0)

/* The number of (512 byte) segments: */
#define N_SEGMENTS (AREA_LEN / 512)

/* We split each segment into "chunks", which are 16 bytes in size.
   These are the smallest blocks that we handle. */
#define CHUNK_SIZE 16
#define N_CHUNKS (AREA_LEN/CHUNK_SIZE)

/* Extra chunks for the vector tables */
#define LAST_CHUNK N_CHUNKS + (64 / (CHUNK_SIZE)) - 1

void flash_init( void );

/* Take a 16-byte chunk of new firmware, and its chunk number.
   The chunk must have been verified as non-erroneous before reaching
   this function. */
void flash_rx_chunk( uint16_t cnum, const uint16_t *fw);

/* Erase a segment starting at addr */
void flash_erase_segment( uint16_t *addr );

/* Get the address of chunk number N of the currently executing firmware */
uint16_t flash_chunk_n( uint16_t n );

/* Current firmware version */
const extern uint16_t FIRMWARE_VERSION;

/* The next firmware chunk that's required */
extern uint16_t next_chunk;

#endif	/* __FLASH_H */
