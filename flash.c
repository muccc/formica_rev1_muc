#include "flash.h"
#include "device.h"
#include "leds.h"

/* Values of the registers */
#define FCTL1_VAL 0
		/* BLKWRT = 0: Byte/word write mode (not block) */
		/* WRT = 0: Not in write mode at the moment */
		/* EEIEX  = 0: No interrupts during write */
		/* EEI = 0: No interrupts during erase */
		/* MERAS:ERASE = 0: No erase. */;

#define FCTL2_VAL (FSSEL_SMCLK | 43)
		/* Flash controller clocked by SMCLK */
		/* Divide the 16 MHz DCO to 363 KHz: */

#define FCTL3_VAL 0

/* Lock the flash against writes and erasure */
#define flash_lock() do { FCTL3 = FCTL3_VAL | FWKEY | LOCK; } while (0)
#define flash_unlock() do { FCTL3 = FCTL3_VAL | FWKEY; } while (0)
#define flash_busy() ( FCTL3 & BUSY )

#define flash_write_mode() do { FCTL1 = FCTL1_VAL | FWKEY | WRT; } while (0)

/* Get a pointer to the segment that x is in */
#define mem_segment(x) ( (uint16_t*)(((uint16_t)x) & 0xfe00 ) )

/* The next chunk to be written */
#if CHUNK_SIZE != 16
#error The type of the following variable needs to be changed for CHUNK_SIZE != 16
#endif
static uint16_t next_chunk;

/* Pointer to the last area to be erased */
static uint16_t *last_erased;

/* Pointer into the other area */
static uint16_t *other_area;
/* Pointer into this area */
static uint16_t *this_area;

const uint16_t FIRMWARE_VERSION = FW_VER;

void flash_init( void )
{
	FCTL1 = FCTL1_VAL | FWKEY;
	FCTL2 = FCTL2_VAL | FWKEY;

	flash_lock();

	/* Nothing's been written yet */
	next_chunk = 0;

	/* Calculate which area we need to write to */
	if( (uint16_t)mem_segment( flash_init ) >= AREA_1 )
	{
		other_area = (uint16_t*)AREA_0;
		this_area = (uint16_t*)AREA_1;
	}
	else
	{
		other_area = (uint16_t*)AREA_1;
		this_area = (uint16_t*)AREA_0;
	}

	last_erased = mem_segment(other_area - 1);
}

void flash_erase_segment( uint16_t *addr )
{
	flash_unlock();

	/* Segment erase -> MERAS = 0, ERASE = 1 */
	FCTL1 = (FCTL1 & (~FRKEY)) | FWKEY | ERASE;

	/* Initiate the erase with a dummy write */
	*addr = 0xffff;

	while( flash_busy() );

	flash_lock();

	last_erased = mem_segment(addr);
}

void flash_rx_chunk( uint16_t cnum, const uint16_t *fw)
{
	uint8_t i;
	/* Pointer to the chunk */
	uint16_t *c = other_area + ((CHUNK_SIZE/2) * cnum);

	/* Discard things that are out of range */
	if( cnum >= N_CHUNKS || cnum != next_chunk )
		return;

	/* Check that the section has been erased */
	if( last_erased < mem_segment( c ) )
		flash_erase_segment( c );

	flash_unlock();
	flash_write_mode();
	/* Now write it */
	for( i=0; i<(CHUNK_SIZE/2); i++ )
		*(c + i) = *(fw + i);
	flash_lock();

	next_chunk++;

	if( cnum == N_CHUNKS-1 )
		leds_green_on();
}

const uint16_t* flash_chunk_n( uint16_t n )
{
	return this_area + ((CHUNK_SIZE/2) * n);
}
