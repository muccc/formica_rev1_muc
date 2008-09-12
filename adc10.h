/*  Copyright 2008 Stephen English, Jeffrey Gough, Alexis Johnson, 
        Robert Spanton and Joanna A. Sun.

    This file is part of the Formica robot firmware.

    Foobar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Formica robot firmware.  
    If not, see <http://www.gnu.org/licenses/>.  */
#ifndef __ADC_10_H
#define __ADC_10_H
#include <stdint.h>

/* Standard init function */
void adc10_init( void );

/* Read the temperature. Must be after _init whilst ADC is disabled*/
uint16_t adc10_readtemp( void );

/* Configure the ADC ready to start streaming out in TX downtime */
void adc10_stream( void );

/* Grab a single reading */
void adc10_grab( void );

/* The readings from the photodiodes */
extern uint16_t pd_value[3];

/* Which photodiode is which */
#define PD_FRONT 0
#define PD_RIGHT 1
#define PD_LEFT 2

#define pd_front (pd_value[PD_FRONT])
#define pd_right (pd_value[PD_RIGHT])
#define pd_left (pd_value[PD_LEFT])

#endif	/* __ADC_10_H */
