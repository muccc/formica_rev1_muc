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
/* Routines that generate the sequences of symbols and bytes for IR 
   transmission. */
#ifndef __IR_TX_DATA_H
#define __IR_TX_DATA_H
#include <stdint.h>

void ir_tx_data_init( void );

/* Generates the next symbol to transmit */
uint8_t ir_tx_next_symbol( void );

#endif	/* __IR_TX_DATA_H */
