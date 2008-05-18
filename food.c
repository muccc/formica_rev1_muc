/*
Food sensor Code, for now just contains a callback function
*/
#include "food.h"

#define AVERAGE_SIZE 3

uint8_t gotfood;
uint16_t average[AVERAGE_SIZE];
uint16_t mavg;
uint8_t averageindex;

/*Need to implement a moving  average based on the last 1sec? any major change from this average will trigger the food detection*/
void foodCallback(uint16_t data, uint8_t channel){
	uint8_t i;
	if(data+50 < mavg){ //checking that the average buffer is full
		gotfood = 1;
	}else{
		gotfood = 0;
	
		/*Calculating moving average*/
		average[averageindex] = data;
		mavg = 0;
		for(i=0; i<AVERAGE_SIZE; i++){
			mavg = mavg+ average[i];
		}
		mavg = mavg/AVERAGE_SIZE;
		/*not using modulo*/
		averageindex++;
		if(averageindex ==AVERAGE_SIZE ){
			averageindex = 0;
		}
	}
}

uint8_t hasfood(void){
	return gotfood;
}
void food_init(void){
	gotfood = 0;
	fled_init();
	fled_on();
}
