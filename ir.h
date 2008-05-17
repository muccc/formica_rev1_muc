/* Things common to both the receive and transmit sides of the IR */
#ifndef __IR_H
#define __IR_H

/* Invalid symbol */
#define INV_SYM 255

/* Nudge the IR transceiver */
/* This should be called at regular intervals, 
   and turns the tx back on after rx'ing for a certain amount of time */
void ir_nudge( void );


#endif	/* __IR_H */
