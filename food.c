#include "types.h"
#include "food.h"
#include "time.h"
#include "leds.h"

#define AVERAGE_SIZE 3
#define FOOD_TRIGGER 500

static uint16_t average[AVERAGE_SIZE];
static bool gotfood = FALSE;

uint32_t food_level = 0;

void foodcallback(uint16_t dataoff, uint16_t dataon)
{
	static uint8_t averageindex = 0;
	uint8_t i;
	uint16_t data;

	/* Dataoff should always be higher than dataon */
	if(dataoff < dataon)
		return;
	data = dataoff - dataon;

	/* Save the data in the array */
	average[averageindex++] = data;
	if(averageindex == AVERAGE_SIZE ){
		averageindex = 0;
	}

	/*Calculating moving average*/
	uint16_t mavg = 0;
	for(i=0; i<AVERAGE_SIZE; i++)
		mavg += average[i];

	mavg = mavg/AVERAGE_SIZE;
	
	if(mavg > FOOD_TRIGGER)
	{
		gotfood = TRUE;
		food_level = 0;
	}
	else
		gotfood = FALSE;
}

bool hasfood(void)
{
	return gotfood;
}

void food_init(void)
{
	uint16_t i;
	for(i=0;i<AVERAGE_SIZE;i++)
		average[i] = 0;

	fled_off();
	P4DIR |= FLED;
}

void food_gotinfo( uint32_t *since )
{
	food_level = (food_level + *since) >> 1;
}

