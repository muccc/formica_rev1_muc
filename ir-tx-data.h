/* Routines that generate the sequences of symbols and bytes for IR 
   transmission. */
#ifndef __IR_TX_DATA_H
#define __IR_TX_DATA_H
#include <stdint.h>

void ir_tx_data_init( void );

/* Generates the next symbol to transmit */
uint8_t ir_tx_next_symbol( void );

#endif	/* __IR_TX_DATA_H */
