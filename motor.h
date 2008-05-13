#ifndef __MOTOR_H
#define __MOTOR_H
#include <stdint.h>

void motor_init( void );

/* Motor directions */
typedef enum {
	FWD,
	BK
} motor_dir_t;

/* Direction of each motor */
typedef struct 
{
	motor_dir_t right : 1;
	motor_dir_t left : 1;
} mdir_t;

extern mdir_t motor_dir;

/* The motor speeds */
#define MAX_SPEED 8
extern uint8_t motor_r;
extern uint8_t motor_l;

#endif	/* __MOTOR_H */
