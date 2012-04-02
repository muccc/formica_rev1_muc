#include "leds.h"
#include "time.h"
#include "device.h"

/* produce a single flash of a colour, as defined in leds.h */

mood_t mood = MOOD_NONE;
mood_t tempmood = MOOD_NONE;
#define FLASHTIME 3

void leds_flash(uint8_t colour)
{
	uint8_t prev_led_state = 0;

	prev_led_state = P4OUT & 0x06;
	P4OUT &= ~(0x06);
	P4OUT |= (colour & 0x06);
	time_wait(FLASHTIME);
	P4OUT &= ~(0x06);
	P4OUT |= prev_led_state;
}

void leds_set(uint8_t colour)
{
	P4OUT &= ~(0x06);
	P4OUT |= (colour & 0x06);
}

