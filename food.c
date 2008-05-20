/*
Food sensor Code, for now just contains a callback function
*/
#include "types.h"
#include "food.h"

#define AVERAGE_SIZE 16
#define FOOD_TRIGGER 100

static uint16_t average[AVERAGE_SIZE];
static bool gotfood = FALSE;

void foodcallback(uint16_t dataoff, uint16_t dataon)
{
	static uint8_t averageindex = 0;
	uint8_t i;
	uint8_t data;

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
	
	if(mavg < FOOD_TRIGGER)
		gotfood = TRUE;
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
	fled_init();
}
