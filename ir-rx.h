#ifndef __IR_RECEIVE_H
#define __IR_RECEIVE_H
#include <stdint.h>

void ir_receive_init( void );

/* Enable/disable IR reception */
void ir_receive_en( void );
void ir_receive_dis( void );

#endif	/* __IR_RECEIVE_H */
