#ifndef __MOTOR_H
#define __MOTOR_H
#include <stdint.h>

void motor_init( void );

typedef enum {
	MOTOR_FWD,
	MOTOR_BK,
	MOTOR_TURN_LEFT,
	MOTOR_TURN_RIGHT
} motor_mode_t;

extern motor_mode_t motor_mode;

/* The motor speeds */
#define MAX_SPEED 8
extern uint8_t motor_r;
extern uint8_t motor_l;

/* Change to another direction for the random walk */
void motor_rand_walk_change( void );

/* Enable/disable the random walk */
void random_walk_enable( void );
void random_walk_disable( void );

/* Set the power ratio of the motors.
   Arguments:
    - l: The weighting of the left motor
    - r: The weighting of the right motor */
void motor_set_ratio(uint16_t l, uint16_t r);

#endif	/* __MOTOR_H */
