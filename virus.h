#ifndef __VIRUS_H
#define __VIRUS_H
#include <stdint.h>

void virus_init( void );

void virus_set( uint8_t version, uint8_t colours );

extern uint8_t virus_colours;
extern uint8_t virus_version;

#endif	/* __VIRUS_H */
