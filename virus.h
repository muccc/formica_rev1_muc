#ifndef __VIRUS_H
#define __VIRUS_H
#include <stdint.h>

void virus_init( void );

void virus_set( uint8_t version, uint8_t colours );

extern uint8_t cur_led_version;
extern uint8_t cur_leds;

#endif	/* __VIRUS_H */
