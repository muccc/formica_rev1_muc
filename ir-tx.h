#ifndef __IR_TRANSMIT_H
#define __IR_TRANSMIT_H
#include "types.h"

void ir_transmit_init( void );

/* Enable IR transmission */
void ir_transmit_enable( void );

/* Returns TRUE if IR transmission is enabled */
bool ir_transmit_is_enabled( void );

#endif	/* __IR_TRANSMIT_H */
