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
extern uint8_t MAX_SPEED;
extern uint8_t motor_r;
extern uint8_t motor_l;

/* Change to another direction for the random walk */
void motor_rand_walk_change( void );

/* Enable/disable the random walk */
void random_walk_enable( void );
void random_walk_disable( void );

#endif	/* __MOTOR_H */
