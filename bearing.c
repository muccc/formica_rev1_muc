#include "device.h"
#define MALENGTH 16

static uint16_t history[MALENGTH];
static uint16_t curbearing;

void setbearing(uint16_t a, uint16_t b, uint16_t c)
{
	static uint16_t hispos = 0;

	if(a > b)
		if(a > c)
			history[hispos] = 0;
		else
			history[hispos] = 240;
	else
		if(b > c)
			history[hispos] = 120;
		else
			history[hispos] = 240;
	
	if(++hispos == MALENGTH)
		hispos = 0;

	uint16_t sum = 0;
	uint16_t i = 0;
	for(i=0;i<MALENGTH;i++)
		sum += history[i];
	
	curbearing = sum/MALENGTH;
}

uint16_t getbearing( void )
{
	return curbearing;	
}
