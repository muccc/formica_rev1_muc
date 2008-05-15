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

#endif	/* __MOTOR_H */
