#include "leds.h"
#include "time.h"
#include "device.h"

/* produce a single flash of a colour, as defined in leds.h */

mood_t mood = MOOD_NONE;

void leds_flash(uint8_t colour)
{
  uint8_t prev_led_state = 0;

  prev_led_state = P4OUT & 0x06;
  P4OUT &= ~(0x06);
  P4OUT |= (colour & 0x06);
  time_wait(2);
  P4OUT &= ~(0x06);
  P4OUT |= prev_led_state;
}

void leds_set(uint8_t colour)
{
  P4OUT &= ~(0x06);
  P4OUT |= (colour & 0x06);
}

void leds_update_mood()
{
  switch (mood)
    {
    case MOOD_NONE:
      leds_set(NONE);
      break;
    case MOOD_DRIVING_TO_CHARGER_NOFOOD:
      leds_flash(GREEN);
      leds_flash(NONE);
      break;
    case MOOD_DRIVING_TO_CHARGER_FLATBATT:
      leds_flash(RED);
      leds_flash(NONE);
      break;
    case MOOD_CHARGING:
      leds_flash(ORANGE);
      leds_flash(NONE);
      break;
    case MOOD_GOT_FOOD:
      leds_set(GREEN);
      break;
    case MOOD_AT_LAMP:
      leds_set(RED);
    default:
      mood = MOOD_NONE;
    }
}



