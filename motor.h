/*  Copyright 2008 Stephen English, Jeffrey Gough, Alexis Johnson, 
        Robert Spanton and Joanna A. Sun.

    This file is part of the Formica robot firmware.

    The Formica robot firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Formica robot firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Formica robot firmware.  
    If not, see <http://www.gnu.org/licenses/>.  */
#ifndef __MOTOR_H
#define __MOTOR_H
#include <stdint.h>

#define M1 (1<<0)
#define MP (1<<1)
#define M2 (1<<3)
#define MN (1<<4)

#define M_ALL (M1 | MP | M2 | MN)

/* Both motors forward */
#define M_FWD ( M1 | M2 )
/* Both motors backward */
#define M_BK ( MP | MN )

/* Left hand motor forward */             //never used!
#define M_L_FWD ( MP | MN | M2 )
/* Left hand motor backward */
#define M_L_BK  ( M1 )

/* Right hand motor forward */            //never used!
#define M_R_FWD ( M1 | MP | MN )
/* Right hand motor backward */
#define M_R_BK ( M2 )

#define motor_off() do { P1DIR &= ~M_ALL; } while (0)
#define motor_fwd() do { P1DIR &= ~M_ALL; P1DIR |= M_FWD; } while (0)
#define motor_bk() do { P1DIR &= ~M_ALL; P1DIR |= M_BK; } while (0)

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
