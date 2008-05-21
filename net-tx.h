/* Deals with the network layer of things */
#ifndef __NET_TX_H
#define __NET_TX_H
#include <stdint.h>

void net_tx_init( void );

/* Returns the next packet to be sent */
const uint8_t* net_tx_get_next_packet( uint8_t *len );

/* Enable transmission for n packets */
void net_tx_enable_for( uint8_t n );

/* Hint which chunk of firmware to transmit next */
void net_tx_chunk_hint( uint16_t chunk );

#endif	/* __NET_TX_H */
