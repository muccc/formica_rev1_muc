#include "braitenberg.h"
#include "../motor.h"
#include "../adc10.h"
#include "../bearing.h"

void braitenberg_update( void )
{
	switch(bearing)
	{
	case 0:
		motor_mode = MOTOR_FWD;

		if(pd_left>pd_right)
		{
			motor_l = 5;
			motor_r = 1;
		}
		else if (pd_right > pd_left)
		{
			motor_r = 5;
			motor_l = 1;
		}
		else
			motor_r = motor_l = 3;
		break;

	case 120:
		motor_mode = MOTOR_TURN_RIGHT;
		motor_r = motor_l = 5;
		break;

	case 240:
		motor_mode = MOTOR_TURN_LEFT;
		motor_r = motor_l = 5;
		break;

	default:
		break;
	}
}

