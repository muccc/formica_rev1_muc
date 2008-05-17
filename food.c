/*
Food sensor Code, for now just contains a callback function
*/
#include "food.h"


uint8_t gotfood;
uint16_t nofoodref;
uint16_t lastdata;

void foodCallback(uint16_t data, uint8_t channel){
	lastdata = data;
	if(data+50 < nofoodref){
		gotfood = 1;
	}else{
		gotfood = 0;
	}
}

uint8_t hasfood(void){
	return gotfood;
}
void food_init(void){
	gotfood = 0;
	fled_init();
	fled_on();
	while(lastdata ==0){};
	nofoodref = lastdata;
}
